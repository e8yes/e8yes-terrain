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
#include "content/scene.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/common_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/renderer_surface_projection.h"
#include "renderer/renderer_radiance.h"
#include "renderer/renderer_solid_color.h"

namespace e8 {
namespace {

constexpr char const *kRendererConfigFileName = "renderer.rpb";

float Alpha(unsigned window_width) { return 1.0f - std::pow(1 - 0.95f, 1.0f / window_width); }

PipelineKey const kAcquireImagePipeline = "AcquireImage";
PipelineKey const kPresentImagePipeline = "PresentImage";

/**
 * @brief The AcquireImagePipeline class
 */
class AcquireImagePipeline : public CachedPipelineInterface {
  public:
    explicit AcquireImagePipeline(VulkanContext *context);
    ~AcquireImagePipeline();

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;

  private:
    GpuPromise acquire_image_promise_;
};

AcquireImagePipeline::AcquireImagePipeline(VulkanContext *context)
    : CachedPipelineInterface(context), acquire_image_promise_(context) {}

AcquireImagePipeline::~AcquireImagePipeline() {}

PipelineKey AcquireImagePipeline::Key() const { return kAcquireImagePipeline; }

Fulfillment AcquireImagePipeline::Launch(CachedPipelineArgumentsInterface const & /*generic_args*/,
                                         std::vector<GpuPromise *> const &prerequisites,
                                         unsigned completion_signal_count,
                                         PipelineOutputInterface *output) {
    assert(prerequisites.empty());
    assert(output != nullptr);

    // Waits for v-sync.
    unsigned swap_chain_image_index;
    assert(VK_SUCCESS == vkAcquireNextImageKHR(context_->device, context_->swap_chain,
                                               /*timeout=*/std::numeric_limits<uint64_t>::max(),
                                               acquire_image_promise_.signal,
                                               /*fence=*/nullptr, &swap_chain_image_index));
    static_cast<SwapChainPipelineOutput *>(output)->SetSwapChainImageIndex(swap_chain_image_index);

    // Dispatches signals to all child stages.
    Fulfillment fulfillment(/*cmds=*/VK_NULL_HANDLE, context_);

    std::vector<VkSemaphore> signals(completion_signal_count);
    for (unsigned i = 0; i < completion_signal_count; ++i) {
        GpuPromise promise(context_);
        signals[i] = promise.signal;
        fulfillment.child_operations_signal.emplace_back(std::move(promise));
    }

    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    submit.pWaitDstStageMask = &wait_stage;
    submit.pWaitSemaphores = &acquire_image_promise_.signal;
    submit.waitSemaphoreCount = 1;
    submit.pSignalSemaphores = signals.data();
    submit.signalSemaphoreCount = signals.size();

    assert(VK_SUCCESS == vkQueueSubmit(context_->graphics_queue, /*submitCount=*/1, &submit,
                                       fulfillment.completion.signal));

    return fulfillment;
}

struct PresentImageArguments : public CachedPipelineArgumentsInterface {
    PresentImageArguments(SwapChainPipelineOutput *output) : output(output) {}

    SwapChainPipelineOutput *output;
};

/**
 * @brief The PresentImagePipeline class
 */
class PresentImagePipeline : public CachedPipelineInterface {
  public:
    PresentImagePipeline(VulkanContext *context);
    ~PresentImagePipeline();

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;
};

PresentImagePipeline::PresentImagePipeline(VulkanContext *context)
    : CachedPipelineInterface(context) {}

PresentImagePipeline::~PresentImagePipeline() {}

PipelineKey PresentImagePipeline::Key() const { return kPresentImagePipeline; }

Fulfillment PresentImagePipeline::Launch(CachedPipelineArgumentsInterface const &generic_args,
                                         std::vector<GpuPromise *> const &prerequisites,
                                         unsigned completion_signal_count,
                                         PipelineOutputInterface *output) {
    assert(completion_signal_count == 0);
    assert(!prerequisites.empty());
    assert(output == nullptr);

    PresentImageArguments const &args = static_cast<PresentImageArguments const &>(generic_args);

    unsigned swap_chain_image_index = args.output->SwapChainImageIndex();

    std::vector<VkSemaphore> parent_signals(prerequisites.size());
    for (unsigned i = 0; i < prerequisites.size(); ++i) {
        parent_signals[i] = prerequisites[i]->signal;
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pSwapchains = &context_->swap_chain;
    present_info.swapchainCount = 1;
    present_info.pImageIndices = &swap_chain_image_index;
    present_info.pWaitSemaphores = parent_signals.data();
    present_info.waitSemaphoreCount = parent_signals.size();

    assert(VK_SUCCESS == vkQueuePresentKHR(context_->present_queue, &present_info));

    Fulfillment empty_fulfillment(/*cmds=*/VK_NULL_HANDLE, context_);
    return empty_fulfillment;
}

} // namespace

RendererInterface::StagePerformance::StagePerformance()
    : name("INVALID"), last_1_frame_ms(0), last_10_frame_ms(1), last_100_frame_ms(2) {}

RendererInterface::StagePerformance::~StagePerformance() {}

RendererInterface::RendererInterface(unsigned num_stages, VulkanContext *context)
    : context(context), stage_performance_(num_stages + 2),
      final_output_(
          std::make_shared<SwapChainPipelineOutput>(/*with_depth_buffer=*/false, context)),
      first_stage_(final_output_), final_stage_(/*output=*/nullptr),
      mu_(std::make_unique<std::mutex>()) {}

RendererInterface::~RendererInterface() {}

std::vector<RendererInterface::StagePerformance> RendererInterface::GetPerformanceStats() const {
    std::lock_guard guard(*mu_);
    return stage_performance_;
}

PipelineStage *RendererInterface::DoFirstStage() {
    CachedPipelineInterface *pipeline = first_stage_.WithPipeline(
        kAcquireImagePipeline, [this](PipelineOutputInterface * /*output*/) {
            return std::make_unique<AcquireImagePipeline>(this->context);
        });

    first_stage_.Schedule(pipeline,
                          /*args=*/std::make_unique<CachedPipelineArgumentsInterface>(),
                          /*parents=*/std::vector<PipelineStage *>{});

    return &first_stage_;
}

PipelineStage *
RendererInterface::DoFinalStage(PipelineStage *first_stage, PipelineStage *final_color_image_stage,
                                std::vector<PipelineStage *> const &dangling_stages) {
    CachedPipelineInterface *pipeline = final_stage_.WithPipeline(
        kPresentImagePipeline, [this](PipelineOutputInterface * /*output*/) {
            return std::make_unique<PresentImagePipeline>(this->context);
        });

    auto args = std::make_unique<PresentImageArguments>(
        static_cast<SwapChainPipelineOutput *>(first_stage->Output()));

    std::vector<PipelineStage *> dependent_parents{first_stage, final_color_image_stage};
    for (auto dangling_stage : dangling_stages) {
        dependent_parents.push_back(dangling_stage);
    }

    final_stage_.Schedule(pipeline, std::move(args), dependent_parents);

    return &final_stage_;
}

std::unique_ptr<PipelineStage> RendererInterface::FinalColorImageStage() const {
    return std::make_unique<PipelineStage>(final_output_);
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
