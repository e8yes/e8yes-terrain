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

using GpuAndCpuPromise = std::pair<std::unique_ptr<GpuPromise>, std::unique_ptr<CpuPromise>>;

class LogLuminanceConfigurator : public PostProcessorConfiguratorInterface {
  public:
    LogLuminanceConfigurator(HdrColorPipelineOutput const &radiance);
    ~LogLuminanceConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    HdrColorPipelineOutput const &radiance_;
};

LogLuminanceConfigurator::LogLuminanceConfigurator(HdrColorPipelineOutput const &radiance)
    : radiance_(radiance) {}

LogLuminanceConfigurator::~LogLuminanceConfigurator() {}

void LogLuminanceConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = radiance_.ColorAttachments()[0]->view;
}

VkCommandBuffer AllocateCommandBuffer(VulkanContext *context) {
    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmds;
    assert(VK_SUCCESS == vkAllocateCommandBuffers(context->device, &cmds_allocation_info, &cmds));

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(cmds, &command_buffer_begin_info));

    return cmds;
}

GpuAndCpuPromise SubmitCommandBuffer(VkCommandBuffer cmds, GpuPromise const &prerequisites,
                                     VulkanContext *context) {
    assert(VK_SUCCESS == vkEndCommandBuffer(cmds));

    auto gpu_promise = std::make_unique<GpuPromise>(cmds, context);
    auto cpu_promise = std::make_unique<CpuPromise>(context);

    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit.pWaitSemaphores = &prerequisites.signal;
    submit.waitSemaphoreCount = 1;
    submit.pWaitDstStageMask = &wait_stage;
    submit.pSignalSemaphores = &gpu_promise->signal;
    submit.signalSemaphoreCount = 1;
    submit.pCommandBuffers = &cmds;
    submit.commandBufferCount = 1;

    assert(VK_SUCCESS ==
           vkQueueSubmit(context->graphics_queue, /*submitCount=*/1, &submit, cpu_promise->signal));

    return GpuAndCpuPromise(std::move(gpu_promise), std::move(cpu_promise));
}

void ComputeAverage(LogLuminancePipelineOutput const &log_luminance, VulkanContext *context,
                    ExposureEstimationPipelineOutput *exposure_output) {
    VkCommandBuffer cmds = AllocateCommandBuffer(context);

    AverageValueOf(log_luminance.ColorAttachments()[0]->image, log_luminance.Width(),
                   log_luminance.Height(), cmds, exposure_output->ColorAttachments()[0]->image);

    auto [gpu_promise, cpu_promise] = SubmitCommandBuffer(cmds, *log_luminance.Promise(), context);
    exposure_output->AddWriter(std::move(gpu_promise), std::move(cpu_promise));
}

} // namespace

LogLuminancePipelineOutput::LogLuminancePipelineOutput(unsigned width, unsigned height,
                                                       VulkanContext *context)
    : PipelineOutputInterface(width, height, context) {
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

ExposureEstimationPipelineOutput::ExposureEstimationPipelineOutput(VulkanContext *context)
    : PipelineOutputInterface(/*width=*/1, /*height=*/1, context) {
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

struct ExposureEstimationPipeline::ExposureEstimationPipelineImpl {
    ExposureEstimationPipelineImpl(LogLuminancePipelineOutput *log_luminance_output,
                                   DescriptorSetAllocator *desc_set_allocator,
                                   VulkanContext *context);
    ~ExposureEstimationPipelineImpl();

    std::unique_ptr<PostProcessorPipeline> log_luminance_pipeline;
};

ExposureEstimationPipeline::ExposureEstimationPipelineImpl::ExposureEstimationPipelineImpl(
    LogLuminancePipelineOutput *log_luminance_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context) {
    log_luminance_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathLogLuminance, /*input_image_count=*/1, /*push_constant_size=*/0,
        log_luminance_output, desc_set_allocator, context);
}

ExposureEstimationPipeline::ExposureEstimationPipelineImpl::~ExposureEstimationPipelineImpl() {}

ExposureEstimationPipeline::ExposureEstimationPipeline(DescriptorSetAllocator *desc_set_allocator,
                                                       VulkanContext *context)
    : desc_set_allocator_(desc_set_allocator), context_(context), current_output_(nullptr) {}

ExposureEstimationPipeline::~ExposureEstimationPipeline() {}

void ExposureEstimationPipeline::Run(HdrColorPipelineOutput const &radiance,
                                     LogLuminancePipelineOutput *log_luminance_output,
                                     ExposureEstimationPipelineOutput *exposure_output) {
    if (log_luminance_output != current_output_) {
        pimpl_ = std::make_unique<ExposureEstimationPipelineImpl>(log_luminance_output,
                                                                  desc_set_allocator_, context_);
        current_output_ = log_luminance_output;
    }

    LogLuminanceConfigurator configurator(radiance);
    pimpl_->log_luminance_pipeline->Run(configurator, *radiance.Promise());
    ComputeAverage(*log_luminance_output, context_, exposure_output);
}

} // namespace e8
