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
#include <cassert>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_output.h"

namespace e8 {

GpuBarrier::GpuBarrier(VulkanContext *context) : context_(context) {}

GpuBarrier::GpuBarrier(VkSemaphore task_signal, VkCommandBuffer task_cmds, VulkanContext *context)
    : tasks_signal{task_signal}, tasks_cmds{task_cmds}, context_(context) {}

GpuBarrier::~GpuBarrier() {
    for (auto task_signal : tasks_signal) {
        vkDestroySemaphore(context_->device, task_signal, /*pAllocator=*/nullptr);
    }

    if (!tasks_cmds.empty()) {
        vkFreeCommandBuffers(context_->device, context_->command_pool, tasks_cmds.size(),
                             tasks_cmds.data());
    }
}

void GpuBarrier::Merge(std::unique_ptr<GpuBarrier> &&other) {
    for (auto signal : other->tasks_signal) {
        tasks_signal.push_back(signal);
    }
    other->tasks_signal.clear();

    for (auto cmds : other->tasks_cmds) {
        tasks_cmds.push_back(cmds);
    }
    other->tasks_cmds.clear();
}

PipelineOutputInterface::PipelineOutputInterface() : width(0), height(0) {}

PipelineOutputInterface::~PipelineOutputInterface() {}

struct SwapChainPipelineOutput::SwapChainPipelineOutputImpl {
    SwapChainPipelineOutputImpl(bool with_depth_buffer, VulkanContext *context);
    ~SwapChainPipelineOutputImpl();

    std::vector<std::unique_ptr<FrameBufferAttachment>> color_attachments_;
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::vector<std::unique_ptr<FrameBuffer>> frame_buffers_;

    int swap_chain_image_index;

    VulkanContext *context;
};

SwapChainPipelineOutput::SwapChainPipelineOutputImpl::SwapChainPipelineOutputImpl(
    bool with_depth_buffer, VulkanContext *context)
    : swap_chain_image_index(-1), context(context) {
    color_attachments_ = CreateColorAttachmentsForSwapChain(context);
    assert(!color_attachments_.empty());

    if (with_depth_buffer) {
        depth_attachment_ = CreateDepthAttachment(context->swap_chain_image_extent.width,
                                                  context->swap_chain_image_extent.height,
                                                  /*samplable=*/false, context);
        render_pass_ =
            CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachments_[0]->desc},
                             depth_attachment_->desc, context);
    } else {
        render_pass_ =
            CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachments_[0]->desc},
                             /*depth_attachment_desc=*/std::nullopt, context);
    }

    for (auto const &color_attachment : color_attachments_) {
        std::unique_ptr<FrameBuffer> frame_buffer =
            CreateFrameBuffer(*render_pass_, context->swap_chain_image_extent.width,
                              context->swap_chain_image_extent.height,
                              std::vector<VkImageView>{color_attachment->view},
                              /*depth_attachment=*/std::nullopt, context);
        frame_buffers_.emplace_back(std::move(frame_buffer));
    }
}

SwapChainPipelineOutput::SwapChainPipelineOutputImpl::~SwapChainPipelineOutputImpl() {}

SwapChainPipelineOutput::SwapChainPipelineOutput(bool with_depth_buffer, VulkanContext *context)
    : pimpl_(std::make_unique<SwapChainPipelineOutputImpl>(with_depth_buffer, context)) {
    this->width = context->swap_chain_image_extent.width;
    this->height = context->swap_chain_image_extent.height;
}

SwapChainPipelineOutput::~SwapChainPipelineOutput() {}

FrameBuffer *SwapChainPipelineOutput::GetFrameBuffer() const {
    assert(pimpl_->swap_chain_image_index >= 0);
    return pimpl_->frame_buffers_[pimpl_->swap_chain_image_index].get();
}

RenderPass const &SwapChainPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass_; }

FrameBufferAttachment const *SwapChainPipelineOutput::ColorAttachment() const {
    assert(pimpl_->swap_chain_image_index >= 0);
    return pimpl_->color_attachments_[pimpl_->swap_chain_image_index].get();
}

FrameBufferAttachment const *SwapChainPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
}

bool SwapChainPipelineOutput::FinalOutput() const { return true; }

void SwapChainPipelineOutput::SetSwapChainImageIndex(unsigned swap_chain_image_index) {
    pimpl_->swap_chain_image_index = swap_chain_image_index;
}

struct DepthMapPipelineOutput::DepthMapPipelineOutputImpl {
    DepthMapPipelineOutputImpl(unsigned width, unsigned height, VulkanContext *context);
    ~DepthMapPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;

    VulkanContext *context;
};

DepthMapPipelineOutput::DepthMapPipelineOutputImpl::DepthMapPipelineOutputImpl(
    unsigned width, unsigned height, VulkanContext *context) {
    depth_attachment_ = CreateDepthAttachment(width, height, /*samplable=*/true, context);
    render_pass_ = CreateRenderPass(/*color_attachments=*/std::vector<VkAttachmentDescription>(),
                                    depth_attachment_->desc, context);
    frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                      /*color_attachments=*/std::vector<VkImageView>(),
                                      depth_attachment_->view, context);
}

DepthMapPipelineOutput::DepthMapPipelineOutputImpl::~DepthMapPipelineOutputImpl() {}

DepthMapPipelineOutput::DepthMapPipelineOutput(unsigned width, unsigned height,
                                               VulkanContext *context)
    : pimpl_(std::make_unique<DepthMapPipelineOutputImpl>(width, height, context)) {
    this->width = width;
    this->height = height;
}

DepthMapPipelineOutput::~DepthMapPipelineOutput() {}

FrameBuffer *DepthMapPipelineOutput::GetFrameBuffer() const { return pimpl_->frame_buffer_.get(); }

RenderPass const &DepthMapPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass_; }

FrameBufferAttachment const *DepthMapPipelineOutput::ColorAttachment() const { return nullptr; }

FrameBufferAttachment const *DepthMapPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
}

bool DepthMapPipelineOutput::FinalOutput() const { return false; }

} // namespace e8