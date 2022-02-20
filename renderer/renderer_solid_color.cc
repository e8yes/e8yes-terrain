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

#include <chrono>
#include <memory>
#include <vulkan/vulkan.h>

#include "common/tensor.h"
#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/render_pass.h"
#include "renderer/renderer_solid_color.h"

namespace e8 {

class SolidColorRenderer::SolidColorRendererImpl {
  public:
    SolidColorRendererImpl(VulkanContext *context);
    ~SolidColorRendererImpl();

  public:
    RenderPass *GetRenderPass();
    FrameBuffer *GetFrameBuffer(unsigned swap_chain_index);

    VulkanContext *context;

  private:
    std::vector<std::unique_ptr<FrameBufferAttachment>> color_attachments_;
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::vector<std::unique_ptr<FrameBuffer>> frame_buffers_;
};

SolidColorRenderer::SolidColorRendererImpl::SolidColorRendererImpl(VulkanContext *context)
    : context(context) {
    color_attachments_ = CreateColorAttachmentsForSwapChain(context);
    assert(!color_attachments_.empty());
    depth_attachment_ = CreateDepthAttachment(context->swap_chain_image_extent.width,
                                              context->swap_chain_image_extent.height, context);
    render_pass_ =
        CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachments_[0]->desc},
                         depth_attachment_->desc, context);

    for (auto const &color_attachment : color_attachments_) {
        std::unique_ptr<FrameBuffer> frame_buffer = CreateFrameBuffer(
            *render_pass_, context->swap_chain_image_extent.width,
            context->swap_chain_image_extent.height,
            std::vector<VkImageView>{color_attachment->view}, depth_attachment_->view, context);
        frame_buffers_.emplace_back(std::move(frame_buffer));
    }
}

SolidColorRenderer::SolidColorRendererImpl::~SolidColorRendererImpl() {}

RenderPass *SolidColorRenderer::SolidColorRendererImpl::GetRenderPass() {
    return render_pass_.get();
}

FrameBuffer *SolidColorRenderer::SolidColorRendererImpl::GetFrameBuffer(unsigned swap_chain_index) {
    return frame_buffers_[swap_chain_index].get();
}

SolidColorRenderer::SolidColorRenderer(VulkanContext *context)
    : pimpl_(std::make_unique<SolidColorRendererImpl>(context)) {}

SolidColorRenderer::~SolidColorRenderer() {}

void SolidColorRenderer::DrawFrame(vec3 const &color) {
    std::unique_ptr<StartFrameResult> start_frame_result = StartFrame(pimpl_->context);

    FrameBuffer *frame_buffer = pimpl_->GetFrameBuffer(start_frame_result->swap_chain_image_index);
    frame_buffer->clear_values[0].color.float32[0] = color(0);
    frame_buffer->clear_values[0].color.float32[1] = color(1);
    frame_buffer->clear_values[0].color.float32[2] = color(2);

    VkCommandBuffer cmds =
        StartRenderPass(*pimpl_->GetRenderPass(), *frame_buffer, pimpl_->context);
    std::unique_ptr<GpuBarrier> final_task = FinishRenderPass(
        cmds, start_frame_result->acquire_swap_chain_image_barrier, /*last=*/true, pimpl_->context);

    EndFrame(*final_task, start_frame_result->swap_chain_image_index,
             /*max_frame_duration=*/std::chrono::seconds(10), pimpl_->context);
}

} // namespace e8
