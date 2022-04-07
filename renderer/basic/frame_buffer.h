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

#ifndef ISLANDS_RENDERER_FRAME_BUFFER_H
#define ISLANDS_RENDERER_FRAME_BUFFER_H

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/render_pass.h"

namespace e8 {

/**
 * @brief The FrameBuffer struct A frame buffer stores the output of a render pass. It
 * will clean up the frame buffer resource by the end of its lifecycle.
 */
struct FrameBuffer {
    /**
     * @brief FrameBuffer Should be created only by calling CreateFrameBuffer().
     */
    FrameBuffer(unsigned width, unsigned height, VulkanContext *context);
    ~FrameBuffer();

    FrameBuffer(FrameBuffer const &) = delete;
    FrameBuffer(FrameBuffer &&) = delete;

    // A full Vulkan object storing information of a frame buffer.
    VkFramebuffer buffer;

    // The dimension of the frame buffer.
    unsigned width;
    unsigned height;

    // The values frame buffer attachments will be initialized to at the beginning of a render pass.
    std::vector<VkClearValue> clear_values;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateFrameBuffer Creates a frame buffer for storing the output of a render pass. This
 * function will always return a valid FrameBuffer structure. Any failure occurs during the frame
 * buffer creation will make it fail.
 *
 * @param render_pass The render pass where the frame buffer associates to.
 * @param width The width, in pixels, of the target rendering area.
 * @param height The height, in pixels, of the target rendering area.
 * @param color_attachments The set of image view of the color attachments the frame buffer
 * contains.
 * @param depth_attachment The image view of the depth attachment, if there is any, the frame buffer
 * contains.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBuffer structure.
 */
std::unique_ptr<FrameBuffer> CreateFrameBuffer(RenderPass const &render_pass, unsigned width,
                                               unsigned height,
                                               std::vector<VkImageView> const &color_attachments,
                                               std::optional<VkImageView> const &depth_attachment,
                                               VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_FRAME_BUFFER_H
