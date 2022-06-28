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
#include <memory>
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/mipmap.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pass/configurator.h"
#include "renderer/pass/rasterize.h"
#include "renderer/postprocessor/exposure.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

PipelineKey const kLogarithmicLuminancePipeline = "Logarithmic Luminance";
PipelineKey const kComputeAveragePipeline = "Compute Average";

using GpuAndCpuPromise = std::pair<std::unique_ptr<GpuPromise>, std::unique_ptr<CpuPromise>>;

/**
 * @brief The LogLuminanceOutput class Stores the logarithmic luminance value of each radiance
 * pixel.
 */
class LogLuminancePipelineOutput : public PipelineOutputInterface {
  public:
    LogLuminancePipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~LogLuminancePipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    std::unique_ptr<FrameBufferAttachment> color_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

LogLuminancePipelineOutput::LogLuminancePipelineOutput(unsigned width, unsigned height,
                                                       VulkanContext *context)
    : PipelineOutputInterface(width, height) {
    color_attachment_ = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R16_SFLOAT,
                                              /*transfer_src=*/true, context);
    render_pass_ = CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachment_->desc},
                                    /*depth_attachment_desc=*/std::nullopt, context);
    frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                      std::vector<VkImageView>{color_attachment_->view},
                                      /*depth_attachment_desc=*/std::nullopt, context);
}

LogLuminancePipelineOutput::~LogLuminancePipelineOutput() {}

FrameBuffer *LogLuminancePipelineOutput::GetFrameBuffer() const { return frame_buffer_.get(); }

RenderPass const &LogLuminancePipelineOutput::GetRenderPass() const { return *render_pass_; }

std::vector<FrameBufferAttachment const *> LogLuminancePipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{color_attachment_.get()};
}

FrameBufferAttachment const *LogLuminancePipelineOutput::DepthAttachment() const { return nullptr; }

/**
 * @brief The ExposureEstimationPipelineOutput class Stores the logarithmic lumiance average in a
 * 1x1 image as the exposure of the radiance map.
 */
class ExposureEstimationPipelineOutput : public PipelineOutputInterface {
  public:
    explicit ExposureEstimationPipelineOutput(VulkanContext *context);
    ~ExposureEstimationPipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    std::unique_ptr<FrameBufferAttachment> color_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
};

ExposureEstimationPipelineOutput::ExposureEstimationPipelineOutput(VulkanContext *context)
    : PipelineOutputInterface(/*width=*/1, /*height=*/1) {
    color_attachment_ = CreateStorageAttachment(/*width=*/1, /*height=*/1,
                                                /*format=*/VK_FORMAT_R16_SFLOAT, context);
    render_pass_ = std::make_unique<RenderPass>(context);
}

ExposureEstimationPipelineOutput::~ExposureEstimationPipelineOutput() {}

FrameBuffer *ExposureEstimationPipelineOutput::GetFrameBuffer() const { return nullptr; }

RenderPass const &ExposureEstimationPipelineOutput::GetRenderPass() const { return *render_pass_; }

std::vector<FrameBufferAttachment const *>
ExposureEstimationPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{color_attachment_.get()};
}

FrameBufferAttachment const *ExposureEstimationPipelineOutput::DepthAttachment() const {
    return nullptr;
}

/**
 * @brief The LogLuminanceConfigurator class For setting up the log luminance shader's uniform
 * variables.
 */
class LogLuminanceConfigurator : public PostProcessorConfiguratorInterface {
  public:
    LogLuminanceConfigurator(PipelineOutputInterface const &radiance);
    ~LogLuminanceConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    PipelineOutputInterface const &radiance_;
};

LogLuminanceConfigurator::LogLuminanceConfigurator(PipelineOutputInterface const &radiance)
    : radiance_(radiance) {}

LogLuminanceConfigurator::~LogLuminanceConfigurator() {}

void LogLuminanceConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = radiance_.ColorAttachments()[0]->view;
}

/**
 * @brief The ComputeAveragePipelineArguments struct Arguments to the ComputeAveragePipeline.
 */
struct ComputeAveragePipelineArguments : public CachedPipelineArgumentsInterface {
    ComputeAveragePipelineArguments(PipelineOutputInterface const &log_luminance_map)
        : log_luminance_map(log_luminance_map) {}

    PipelineOutputInterface const &log_luminance_map;
};

/**
 * @brief The ComputeAveragePipeline class For computing the average value of log luminance map.
 */
class ComputeAveragePipeline : public CachedPipelineInterface {
  public:
    ComputeAveragePipeline(VulkanContext *context);
    ~ComputeAveragePipeline() override;

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;

  private:
    VkCommandBuffer AllocateCommandBuffer();
    Fulfillment SubmitCommandBuffer(VkCommandBuffer cmds,
                                    std::vector<GpuPromise *> const &prerequisites,
                                    unsigned completion_signal_count);
};

ComputeAveragePipeline::ComputeAveragePipeline(VulkanContext *context)
    : CachedPipelineInterface(context) {}

ComputeAveragePipeline::~ComputeAveragePipeline() {}

VkCommandBuffer ComputeAveragePipeline::AllocateCommandBuffer() {
    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context_->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmds;
    assert(VK_SUCCESS == vkAllocateCommandBuffers(context_->device, &cmds_allocation_info, &cmds));

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(cmds, &command_buffer_begin_info));

    return cmds;
}

Fulfillment
ComputeAveragePipeline::SubmitCommandBuffer(VkCommandBuffer cmds,
                                            std::vector<GpuPromise *> const &prerequisites,
                                            unsigned completion_signal_count) {
    assert(VK_SUCCESS == vkEndCommandBuffer(cmds));

    // For render pass synchronization.
    std::vector<VkSemaphore> waits(prerequisites.size());
    for (unsigned i = 0; i < prerequisites.size(); ++i) {
        waits[i] = prerequisites[i]->signal;
    }

    Fulfillment fulfillment(cmds, context_);

    std::vector<VkSemaphore> signals(completion_signal_count);
    for (unsigned i = 0; i < completion_signal_count; ++i) {
        GpuPromise promise(context_);
        signals[i] = promise.signal;
        fulfillment.child_operations_signal.emplace_back(std::move(promise));
    }

    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit.pWaitSemaphores = waits.data();
    submit.waitSemaphoreCount = 1;
    submit.pWaitDstStageMask = &wait_stage;
    submit.pSignalSemaphores = signals.data();
    submit.signalSemaphoreCount = 1;
    submit.pCommandBuffers = &cmds;
    submit.commandBufferCount = 1;

    assert(VK_SUCCESS == vkQueueSubmit(context_->graphics_queue, /*submitCount=*/1, &submit,
                                       fulfillment.completion.signal));

    return fulfillment;
}

PipelineKey ComputeAveragePipeline::Key() const { return kComputeAveragePipeline; }

Fulfillment ComputeAveragePipeline::Launch(CachedPipelineArgumentsInterface const &generic_args,
                                           std::vector<GpuPromise *> const &prerequisites,
                                           unsigned completion_signal_count,
                                           PipelineOutputInterface *exposure_output) {
    ComputeAveragePipelineArguments const &args =
        static_cast<ComputeAveragePipelineArguments const &>(generic_args);

    VkCommandBuffer cmds = this->AllocateCommandBuffer();

    AverageValueOf(args.log_luminance_map.ColorAttachments()[0]->image,
                   args.log_luminance_map.Width(), args.log_luminance_map.Height(), cmds,
                   exposure_output->ColorAttachments()[0]->image);

    return this->SubmitCommandBuffer(cmds, prerequisites, completion_signal_count);
}

} // namespace

std::unique_ptr<PipelineStage> CreateLogLuminaneStage(unsigned width, unsigned height,
                                                      VulkanContext *context) {
    auto output = std::make_shared<LogLuminancePipelineOutput>(width, height, context);
    return std::make_unique<PipelineStage>(output);
}

std::unique_ptr<PipelineStage> CreateExposureStage(VulkanContext *context) {
    auto output = std::make_shared<ExposureEstimationPipelineOutput>(context);
    return std::make_unique<PipelineStage>(output);
}

void DoEstimateExposure(PipelineStage *radiance_map, DescriptorSetAllocator *desc_set_allocator,
                        VulkanContext *context, PipelineStage *log_luminance_map,
                        PipelineStage *exposure) {
    CachedPipelineInterface *log_luminance_pipeline = log_luminance_map->WithPipeline(
        kLogarithmicLuminancePipeline,
        [desc_set_allocator, context](PipelineOutputInterface *log_luminance_output) {
            return std::make_unique<PostProcessorPipeline>(
                kLogarithmicLuminancePipeline, kFragmentShaderFilePathLogLuminance,
                /*input_image_count=*/1, /*push_constant_size=*/0, log_luminance_output,
                desc_set_allocator, context);
        });
    CachedPipelineInterface *compute_average_pipeline = exposure->WithPipeline(
        kComputeAveragePipeline, [context](PipelineOutputInterface * /*exposure_output*/) {
            return std::make_unique<ComputeAveragePipeline>(context);
        });

    auto configurator = std::make_unique<LogLuminanceConfigurator>(*radiance_map->Output());
    log_luminance_map->Schedule(log_luminance_pipeline, std::move(configurator),
                                /*parents=*/std::vector<PipelineStage *>{radiance_map});

    auto compute_average_args =
        std::make_unique<ComputeAveragePipelineArguments>(*log_luminance_map->Output());
    exposure->Schedule(compute_average_pipeline, std::move(compute_average_args),
                       /*parents=*/std::vector<PipelineStage *>{log_luminance_map});
}

} // namespace e8
