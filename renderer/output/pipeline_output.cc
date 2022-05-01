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
#include <limits>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"

namespace e8 {

PipelineOutputInterface::PipelineOutputInterface(unsigned width, unsigned height,
                                                 VulkanContext *context)
    : width_(width), height_(height), context_(context) {}

PipelineOutputInterface::~PipelineOutputInterface() {}

unsigned PipelineOutputInterface::Width() const { return width_; }

unsigned PipelineOutputInterface::Height() const { return height_; }

void PipelineOutputInterface::Fulfill() {
    for (auto const &promise : cpu_promises_) {
        assert(VK_SUCCESS == vkWaitForFences(context_->device, /*fenceCount=*/1, &promise->signal,
                                             /*waitAll=*/VK_TRUE,
                                             /*timeout=*/std::numeric_limits<uint64_t>::max()));
    }

    gpu_promises_.clear();
    cpu_promises_.clear();
}

GpuPromise const *PipelineOutputInterface::Promise() const {
    if (gpu_promises_.empty()) {
        return nullptr;
    }
    return gpu_promises_.back().get();
}

void PipelineOutputInterface::AddWriter(std::unique_ptr<GpuPromise> &&gpu_promise,
                                        std::unique_ptr<CpuPromise> &&cpu_promise) {
    gpu_promises_.push_back(std::move(gpu_promise));
    cpu_promises_.push_back(std::move(cpu_promise));
}

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

        for (auto const &color_attachment : color_attachments_) {
            std::unique_ptr<FrameBuffer> frame_buffer =
                CreateFrameBuffer(*render_pass_, context->swap_chain_image_extent.width,
                                  context->swap_chain_image_extent.height,
                                  std::vector<VkImageView>{color_attachment->view},
                                  /*depth_attachment=*/depth_attachment_->view, context);

            frame_buffers_.emplace_back(std::move(frame_buffer));
        }
    } else {
        render_pass_ =
            CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachments_[0]->desc},
                             /*depth_attachment_desc=*/std::nullopt, context);

        for (auto const &color_attachment : color_attachments_) {
            std::unique_ptr<FrameBuffer> frame_buffer =
                CreateFrameBuffer(*render_pass_, context->swap_chain_image_extent.width,
                                  context->swap_chain_image_extent.height,
                                  std::vector<VkImageView>{color_attachment->view},
                                  /*depth_attachment=*/std::nullopt, context);

            frame_buffers_.emplace_back(std::move(frame_buffer));
        }
    }
}

SwapChainPipelineOutput::SwapChainPipelineOutputImpl::~SwapChainPipelineOutputImpl() {}

SwapChainPipelineOutput::SwapChainPipelineOutput(bool with_depth_buffer, VulkanContext *context)
    : PipelineOutputInterface(context->swap_chain_image_extent.width,
                              context->swap_chain_image_extent.height, context),
      pimpl_(std::make_unique<SwapChainPipelineOutputImpl>(with_depth_buffer, context)) {}

SwapChainPipelineOutput::~SwapChainPipelineOutput() {}

FrameBuffer *SwapChainPipelineOutput::GetFrameBuffer() const {
    assert(pimpl_->swap_chain_image_index >= 0);
    return pimpl_->frame_buffers_[pimpl_->swap_chain_image_index].get();
}

RenderPass const &SwapChainPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass_; }

std::vector<FrameBufferAttachment const *> SwapChainPipelineOutput::ColorAttachments() const {
    assert(pimpl_->swap_chain_image_index >= 0);
    return std::vector<FrameBufferAttachment const *>{
        pimpl_->color_attachments_[pimpl_->swap_chain_image_index].get()};
}

FrameBufferAttachment const *SwapChainPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
}

void SwapChainPipelineOutput::SetSwapChainImageIndex(unsigned swap_chain_image_index) {
    pimpl_->swap_chain_image_index = swap_chain_image_index;
}

struct UnboundedColorPipelineOutput::UnboundedColorPipelineOutputImpl {
    UnboundedColorPipelineOutputImpl(unsigned width, unsigned height, bool with_depth_buffer,
                                     VulkanContext *context);
    ~UnboundedColorPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> color_attachment;
    std::unique_ptr<FrameBufferAttachment> depth_attachment;
    std::unique_ptr<RenderPass> render_pass;
    std::unique_ptr<FrameBuffer> frame_buffer;

    VulkanContext *context;
};

UnboundedColorPipelineOutput::UnboundedColorPipelineOutputImpl::UnboundedColorPipelineOutputImpl(
    unsigned width, unsigned height, bool with_depth_buffer, VulkanContext *context) {
    color_attachment = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT,
                                             /*transfer_src=*/false, context);

    if (with_depth_buffer) {
        depth_attachment = CreateDepthAttachment(context->swap_chain_image_extent.width,
                                                 context->swap_chain_image_extent.height,
                                                 /*samplable=*/false, context);

        render_pass = CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachment->desc},
                                       depth_attachment->desc, context);

        frame_buffer = CreateFrameBuffer(*render_pass, width, height,
                                         std::vector<VkImageView>{color_attachment->view},
                                         /*depth_attachment=*/depth_attachment->view, context);
    } else {
        render_pass = CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachment->desc},
                                       /*depth_attachment_desc=*/std::nullopt, context);

        frame_buffer = CreateFrameBuffer(*render_pass, width, height,
                                         std::vector<VkImageView>{color_attachment->view},
                                         /*depth_attachment=*/std::nullopt, context);
    }
}

UnboundedColorPipelineOutput::UnboundedColorPipelineOutputImpl::
    ~UnboundedColorPipelineOutputImpl() {}

UnboundedColorPipelineOutput::UnboundedColorPipelineOutput(unsigned width, unsigned height,
                                                           bool with_depth_buffer,
                                                           VulkanContext *context)
    : PipelineOutputInterface(width, height, context),
      pimpl_(std::make_unique<UnboundedColorPipelineOutputImpl>(width, height, with_depth_buffer,
                                                                context)) {}

UnboundedColorPipelineOutput::~UnboundedColorPipelineOutput() {}

FrameBuffer *UnboundedColorPipelineOutput::GetFrameBuffer() const {
    return pimpl_->frame_buffer.get();
}

RenderPass const &UnboundedColorPipelineOutput::GetRenderPass() const {
    return *pimpl_->render_pass;
}

std::vector<FrameBufferAttachment const *> UnboundedColorPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{pimpl_->color_attachment.get()};
}

FrameBufferAttachment const *UnboundedColorPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment.get();
}

struct LdrColorPipelineOutput::LdrColorPipelineOutputImpl {
    LdrColorPipelineOutputImpl(unsigned width, unsigned height, bool with_depth_buffer,
                               VulkanContext *context);
    ~LdrColorPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> color_attachment;
    std::unique_ptr<FrameBufferAttachment> depth_attachment;
    std::unique_ptr<RenderPass> render_pass;
    std::unique_ptr<FrameBuffer> frame_buffer;

    VulkanContext *context;
};

LdrColorPipelineOutput::LdrColorPipelineOutputImpl::LdrColorPipelineOutputImpl(
    unsigned width, unsigned height, bool with_depth_buffer, VulkanContext *context) {
    color_attachment = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R8G8B8A8_SNORM,
                                             /*transfer_src=*/false, context);

    if (with_depth_buffer) {
        depth_attachment = CreateDepthAttachment(context->swap_chain_image_extent.width,
                                                 context->swap_chain_image_extent.height,
                                                 /*samplable=*/false, context);

        render_pass = CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachment->desc},
                                       depth_attachment->desc, context);

        frame_buffer = CreateFrameBuffer(*render_pass, width, height,
                                         std::vector<VkImageView>{color_attachment->view},
                                         /*depth_attachment=*/depth_attachment->view, context);
    } else {
        render_pass = CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachment->desc},
                                       /*depth_attachment_desc=*/std::nullopt, context);

        frame_buffer = CreateFrameBuffer(*render_pass, width, height,
                                         std::vector<VkImageView>{color_attachment->view},
                                         /*depth_attachment=*/std::nullopt, context);
    }
}

LdrColorPipelineOutput::LdrColorPipelineOutputImpl::~LdrColorPipelineOutputImpl() {}

LdrColorPipelineOutput::LdrColorPipelineOutput(unsigned width, unsigned height,
                                               bool with_depth_buffer, VulkanContext *context)
    : PipelineOutputInterface(width, height, context),
      pimpl_(std::make_unique<LdrColorPipelineOutputImpl>(width, height, with_depth_buffer,
                                                          context)) {}

LdrColorPipelineOutput::~LdrColorPipelineOutput() {}

FrameBuffer *LdrColorPipelineOutput::GetFrameBuffer() const { return pimpl_->frame_buffer.get(); }

RenderPass const &LdrColorPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass; }

std::vector<FrameBufferAttachment const *> LdrColorPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{pimpl_->color_attachment.get()};
}

FrameBufferAttachment const *LdrColorPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment.get();
}

} // namespace e8
