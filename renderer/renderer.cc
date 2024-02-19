/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/dag/promise.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/renderer_radiance.h"
#include "renderer/renderer_solid_color.h"
#include "renderer/renderer_surface_projection.h"

namespace e8 {
namespace {

constexpr char const *kRendererConfigFileName = "renderer.rpb";

float Alpha(unsigned window_width) { return 1.0f - std::pow(1 - 0.95f, 1.0f / window_width); }

} // namespace

RendererInterface::StagePerformance::StagePerformance()
    : name("INVALID"), last_1_frame_ms(0), last_10_frame_ms(1), last_100_frame_ms(2) {}

RendererInterface::StagePerformance::~StagePerformance() {}

RendererInterface::RendererInterface(unsigned num_stages, VulkanContext *context)
    : context(context), stage_performance_(num_stages + 2),
      final_output_(std::make_shared<SwapChainOutput>(/*with_depth_buffer=*/false, context)),
      acquisition_signal_(context), mu_(std::make_unique<std::mutex>()) {}

RendererInterface::~RendererInterface() {}

std::vector<RendererInterface::StagePerformance> RendererInterface::GetPerformanceStats() const {
    std::lock_guard guard(*mu_);
    return stage_performance_;
}

std::shared_ptr<SwapChainOutput>
RendererInterface::AcquireFinalColorImage(FrameResourceAllocator *frame_resource_allocator) {
    acquisition_signal_.Reset();
    unsigned swap_chain_image_index;
    assert(VK_SUCCESS == vkAcquireNextImageKHR(context->device, context->swap_chain,
                                               /*timeout=*/std::numeric_limits<uint64_t>::max(),
                                               /*semaphore=*/VK_NULL_HANDLE,
                                               /*fence=*/acquisition_signal_.signal,
                                               &swap_chain_image_index));
    acquisition_signal_.Wait();

    final_output_->SetSwapChainImageIndex(swap_chain_image_index);
    frame_resource_allocator->SetSwapChainImageIndex(swap_chain_image_index);
    return final_output_;
}

void RendererInterface::PresentFinalColorImage(SwapChainOutput const &output,
                                               std::vector<GpuPromise *> const &final_waits) {
    unsigned swap_chain_image_index = output.SwapChainImageIndex();

    std::vector<VkSemaphore> dependent_signals;
    dependent_signals.reserve(final_waits.size());
    for (auto const &final_wait : final_waits) {
        dependent_signals.push_back(final_wait->signal);
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pSwapchains = &context->swap_chain;
    present_info.swapchainCount = 1;
    present_info.pImageIndices = &swap_chain_image_index;
    present_info.pWaitSemaphores = dependent_signals.data();
    present_info.waitSemaphoreCount = dependent_signals.size();

    assert(VK_SUCCESS == vkQueuePresentKHR(context->present_queue, &present_info));
}

void RendererInterface::BeginStage(unsigned index, std::string const &name) {
    std::lock_guard guard(*mu_);
    assert(index < stage_performance_.size());

    stage_performance_[index].name = name;
    stage_performance_[index].begin_at = std::chrono::high_resolution_clock::now();
}

void RendererInterface::EndStage(unsigned index) {
    std::lock_guard guard(*mu_);

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration time_spent = now - stage_performance_[index].begin_at;
    float time_spent_micros =
        std::chrono::duration_cast<std::chrono::microseconds>(time_spent).count();
    float time_spent_millis = time_spent_micros / 1000.0f;

    stage_performance_[index].last_1_frame_ms = time_spent_millis;

    if (stage_performance_[index].last_10_frame_ms == 0) {
        stage_performance_[index].last_10_frame_ms = time_spent_millis;
    } else {
        stage_performance_[index].last_10_frame_ms =
            Alpha(10) * time_spent_millis +
            (1.0f - Alpha(10)) * stage_performance_[index].last_10_frame_ms;
    }

    if (stage_performance_[index].last_100_frame_ms == 0) {
        stage_performance_[index].last_100_frame_ms = time_spent_millis;
    } else {
        stage_performance_[index].last_100_frame_ms =
            Alpha(100) * time_spent_millis +
            (1.0f - Alpha(100)) * stage_performance_[index].last_100_frame_ms;
    }

    if (stage_performance_[index].last_1000_frame_ms == 0) {
        stage_performance_[index].last_1000_frame_ms = time_spent_millis;
    } else {
        stage_performance_[index].last_1000_frame_ms =
            Alpha(1000) * time_spent_millis +
            (1.0f - Alpha(1000)) * stage_performance_[index].last_1000_frame_ms;
    }
}

std::unique_ptr<RendererInterface> CreateRenderer(RendererType type, VulkanContext *context) {
    switch (type) {
    case RendererType::RT_SOLID_COLOR: {
        return std::make_unique<SolidColorRenderer>(context);
    }
    case RendererType::RT_DEPTH: {
        return std::make_unique<DepthRenderer>(context);
    }
    case RendererType::RT_LIGHT_INPUTS: {
        return std::make_unique<SurfaceProjectionRenderer>(context);
    }
    case RendererType::RT_RADIANCE: {
        return std::make_unique<RadianceRenderer>(context);
    }
    case RendererType::RT_RADIOSITY: {
        assert(false);
    }
    default: {
        assert(false);
    }
    }
}

std::unique_ptr<RendererConfiguration>
LoadRendererConfiguration(std::filesystem::path const &base_path) {
    std::fstream config_file(base_path / kRendererConfigFileName, std::ios::in | std::ios::binary);
    if (!config_file.is_open()) {
        return nullptr;
    }

    auto config = std::make_unique<RendererConfiguration>();
    if (!config->ParseFromIstream(&config_file)) {
        return nullptr;
    }

    return config;
}

bool SaveRendererConfiguration(RendererConfiguration const &config,
                               std::filesystem::path const &base_path) {
    std::fstream config_file(base_path / kRendererConfigFileName,
                             std::ios::out | std::ios::trunc | std::ios::binary);
    if (!config_file.is_open()) {
        return false;
    }

    return config.SerializeToOstream(&config_file);
}

std::unique_ptr<RendererConfiguration> DefaultRendererConfiguration() {
    auto config = std::make_unique<RendererConfiguration>();
    config->set_in_use_renderer_type(RendererType::RT_SOLID_COLOR);
    config->mutable_depth_renderer_params()->set_alpha(1.0f);
    config->mutable_light_inputs_renderer_params()->set_input_to_visualize(
        LightInputsRendererParameters::NORMAL);
    return config;
}

} // namespace e8
