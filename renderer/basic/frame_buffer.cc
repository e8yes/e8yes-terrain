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
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"

namespace e8 {

FrameBuffer::FrameBuffer(unsigned width, unsigned height, VulkanContext *context)
    : width(width), height(height), context_(context) {}

FrameBuffer::~FrameBuffer() {
    vkDestroyFramebuffer(context_->device, buffer, /*pAllocator=*/nullptr);
}

std::unique_ptr<FrameBuffer> CreateFrameBuffer(RenderPass const &render_pass, unsigned width,
                                               unsigned height,
                                               std::vector<VkImageView> const &color_attachments,
                                               std::optional<VkImageView> const &depth_attachment,
                                               VulkanContext *context) {
    auto info = std::make_unique<FrameBuffer>(width, height, context);

    // Creates frame buffer.
    VkFramebufferCreateInfo frame_buffer_info{};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.renderPass = render_pass.pass;
    frame_buffer_info.width = width;
    frame_buffer_info.height = height;
    frame_buffer_info.layers = 1;

    // Collects and sets attachment views.
    std::vector<VkImageView> attachment_views(color_attachments.begin(), color_attachments.end());
    if (depth_attachment.has_value()) {
        attachment_views.push_back(*depth_attachment);
    }

    if (!attachment_views.empty()) {
        frame_buffer_info.pAttachments = attachment_views.data();
        frame_buffer_info.attachmentCount = attachment_views.size();
    }

    assert(VK_SUCCESS == vkCreateFramebuffer(context->device, &frame_buffer_info,
                                             /*pAllocator=*/nullptr, &info->buffer));

    // Set clear values defaults.
    for (unsigned i = 0; i < color_attachments.size(); ++i) {
        VkClearValue color_value{};
        info->clear_values.push_back(color_value);
    }
    if (depth_attachment.has_value()) {
        VkClearValue depth_value{};
        depth_value.depthStencil.depth = 0.0f;
        depth_value.depthStencil.stencil = 0.0f;
        info->clear_values.push_back(depth_value);
    }

    return info;
}

} // namespace e8
