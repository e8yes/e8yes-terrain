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

#include <algorithm>
#include <memory>
#include <vulkan/vulkan.h>

#include "common/tensor.h"
#include "common/device.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_output.h"
#include "renderer/pipeline_solid_color.h"
#include "renderer/render_pass.h"

namespace e8 {

struct SolidColorPipeline::SolidColorPipelineImpl {
    SolidColorPipelineImpl(PipelineOutputInterface *output, VulkanContext *context);
    ~SolidColorPipelineImpl();

    PipelineOutputInterface *output;
    VulkanContext *context;
};

SolidColorPipeline::SolidColorPipelineImpl::SolidColorPipelineImpl(PipelineOutputInterface *output,
                                                                   VulkanContext *context)
    : output(output), context(context) {}

SolidColorPipeline::SolidColorPipelineImpl::~SolidColorPipelineImpl() {}

SolidColorPipeline::SolidColorPipeline(PipelineOutputInterface *output, VulkanContext *context)
    : pimpl_(std::make_unique<SolidColorPipelineImpl>(output, context)) {}

SolidColorPipeline::~SolidColorPipeline() {}

PipelineOutputInterface *SolidColorPipeline::Run(vec3 const &color,
                                                 GpuBarrier const &prerequisites) {
    FrameBuffer *frame_buffer = pimpl_->output->GetFrameBuffer();

    frame_buffer->clear_values[0].color.float32[0] = color(0);
    frame_buffer->clear_values[0].color.float32[1] = color(1);
    frame_buffer->clear_values[0].color.float32[2] = color(2);

    VkCommandBuffer cmds =
        StartRenderPass(pimpl_->output->GetRenderPass(), *frame_buffer, pimpl_->context);
    pimpl_->output->barrier =
        FinishRenderPass(cmds, prerequisites, pimpl_->output->AcquireFence(), pimpl_->context);

    return pimpl_->output;
}

} // namespace e8
