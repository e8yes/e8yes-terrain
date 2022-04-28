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
#include "renderer/basic/render_pass.h"

namespace e8 {

RenderPass::RenderPass(VulkanContext *context) : pass(VK_NULL_HANDLE), context_(context) {}

RenderPass::~RenderPass() {
    if (pass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(context_->device, pass, /*pAllocator=*/nullptr);
    }
}

std::unique_ptr<RenderPass>
CreateRenderPass(std::vector<VkAttachmentDescription> const &color_attachment_descs,
                 std::optional<VkAttachmentDescription> const &depth_attachment_desc,
                 VulkanContext *context) {
    auto info = std::make_unique<RenderPass>(context);

    // Creates render pass & subpass.
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    unsigned color_attachment_count = color_attachment_descs.size();

    // Collects and sets attachment references.
    std::vector<VkAttachmentReference> color_attachment_refs;
    for (unsigned i = 0; i < color_attachment_count; ++i) {
        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        color_attachment_ref.attachment = i;

        color_attachment_refs.push_back(color_attachment_ref);
    }
    if (!color_attachment_refs.empty()) {
        subpass.pColorAttachments = color_attachment_refs.data();
        subpass.colorAttachmentCount = color_attachment_refs.size();
    }

    VkAttachmentReference depth_attachment_ref{};
    if (depth_attachment_desc.has_value()) {
        unsigned depth_attachment = color_attachment_count;
        depth_attachment_ref.attachment = depth_attachment;
        depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        subpass.pDepthStencilAttachment = &depth_attachment_ref;
    }

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.subpassCount = 1;

    // Collects and sets attachment descriptions.
    std::vector<VkAttachmentDescription> attachment_descs(color_attachment_descs.begin(),
                                                          color_attachment_descs.end());
    if (depth_attachment_desc.has_value()) {
        attachment_descs.push_back(*depth_attachment_desc);
    }
    if (!attachment_descs.empty()) {
        render_pass_info.pAttachments = attachment_descs.data();
        render_pass_info.attachmentCount = attachment_descs.size();
    }

    assert(VK_SUCCESS == vkCreateRenderPass(context->device, &render_pass_info,
                                            /*pAllocator=*/nullptr, &info->pass));

    return info;
}

} // namespace e8
