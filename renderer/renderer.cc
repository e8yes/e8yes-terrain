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
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "content/scene.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/renderer_solid_color.h"

namespace e8 {
namespace {

constexpr char const *kRendererConfigFileName = "renderer.rpb";

float Alpha(unsigned window_width) { return 1.0f - std::pow(1 - 0.95f, 1.0f / window_width); }

} // namespace

RendererInterface::StagePerformance::StagePerformance()
    : name("INVALID"), last_1_frame_ms(0), last_10_frame_ms(1), last_100_frame_ms(2) {}

RendererInterface::StagePerformance::~StagePerformance() {}

RendererInterface::FrameContext::FrameContext(VulkanContext *context)
    : acquire_swap_chain_image_barrier(context), swap_chain_image_index(0) {}

RendererInterface::RendererInterface(unsigned num_stages,
                                     std::chrono::nanoseconds const &max_frame_duration,
                                     VulkanContext *context)
    : context(context), max_frame_duration(max_frame_duration), stage_performance_(num_stages + 2),
      mu_(std::make_unique<std::mutex>()) {}

RendererInterface::~RendererInterface() {}

std::vector<RendererInterface::StagePerformance> RendererInterface::GetPerformanceStats() const {
    std::lock_guard guard(*mu_);
    return stage_performance_;
}

RendererInterface::FrameContext RendererInterface::BeginFrame() {
    FrameContext frame_context(context);

    VkSemaphore done_signal;
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    assert(VK_SUCCESS == vkCreateSemaphore(context->device, &semaphore_info,
                                           /*pAllocator=*/nullptr, &done_signal));

    this->BeginStage(/*index=*/0, /*name=*/"Acquire Next Image");
    assert(VK_SUCCESS == vkAcquireNextImageKHR(context->device, context->swap_chain,
                                               /*timeout=*/UINT64_MAX, done_signal,
                                               /*fence=*/nullptr,
                                               &frame_context.swap_chain_image_index));
    frame_context.acquire_swap_chain_image_barrier.tasks_signal.push_back(done_signal);
    this->EndStage(/*index=*/0);

    return frame_context;
}

void RendererInterface::EndFrame(FrameContext const &frame_context,
                                 PipelineOutputInterface *final_ouput) {
    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pSwapchains = &context->swap_chain;
    present_info.swapchainCount = 1;
    present_info.pImageIndices = &frame_context.swap_chain_image_index;
    if (!final_ouput->barrier->tasks_signal.empty()) {
        present_info.pWaitSemaphores = final_ouput->barrier->tasks_signal.data();
        present_info.waitSemaphoreCount = final_ouput->barrier->tasks_signal.size();
    }

    assert(VK_SUCCESS == vkQueuePresentKHR(context->present_queue, &present_info));

    this->BeginStage(stage_performance_.size() - 1, /*name=*/"Final Output");
    final_ouput->Fulfill(max_frame_duration);
    this->EndStage(stage_performance_.size() - 1);
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
    case RendererType::RT_RADIANCE: {
        assert(false);
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
    return config;
}

} // namespace e8
