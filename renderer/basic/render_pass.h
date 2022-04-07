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

#ifndef ISLANDS_RENDERER_RENDER_PASS_H
#define ISLANDS_RENDERER_RENDER_PASS_H

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

/**
 * @brief The RenderPassInfo struct Stores a Vulkan object describing the render pass in a graphics
 * pipeline. It will clean up the render pass resource by the end of its lifecycle.
 */
struct RenderPass {
    /**
     * @brief RenderPassInfo Should be created only by calling CreateRenderPass().
     */
    explicit RenderPass(VulkanContext *context);
    ~RenderPass();

    RenderPass(RenderPass const &) = delete;
    RenderPass(RenderPass &&) = delete;

    // A full Vulkan object storing information of a render pass.
    VkRenderPass pass;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateRenderPass Creates a render pass that has exactly one sub-pass. This function will
 * always return a valid RenderPass structure. Any failure occurs during the render pass creation
 * will make it fail.
 *
 * @param color_attachment_descs Color attachments that will receive outputs of a render pass.
 * @param depth_attachment_desc The depth attachment for depth buffering. This argument is required
 * when depth test is enabled in the FixedStageConfig.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBufferAttachment structure.
 */
std::unique_ptr<RenderPass>
CreateRenderPass(std::vector<VkAttachmentDescription> const &color_attachment_descs,
                 std::optional<VkAttachmentDescription> const &depth_attachment_desc,
                 VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_RENDER_PASS_H
