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

#ifndef ISLANDS_RENDERER_ATTACHMENT_H
#define ISLANDS_RENDERER_ATTACHMENT_H

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

/**
 * @brief The FrameBufferAttachment struct Stores the content of a frame buffer attachment,
 * including its memory block. It will clean up the attachment image and image view resources by the
 * end of its lifecycle.
 */
struct FrameBufferAttachment {
    /**
     * @brief FrameBufferAttachmentInfo Should be created only by calling
     * CreateColorAttachmentsForSwapChain() or CreateDepthAttachment.
     */
    FrameBufferAttachment(bool swap_chain_image, VulkanContext *context);
    ~FrameBufferAttachment();

    FrameBufferAttachment(FrameBufferAttachment const &) = delete;
    FrameBufferAttachment(FrameBufferAttachment &&) = delete;

    // Indicates if the underlying image is created along with the swap chain.
    bool swap_chain_image;

    // The memory block which holds the actual image data.
    VkImage image;
    VmaAllocation allocation;

    // Allows the attachment to be referred to in a frame buffer.
    VkImageView view;

    // Allows the attachment to be referred to in a render pass.
    VkAttachmentDescription desc;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateColorAttachmentsForSwapChain Creates a series of frame buffer color attachments for
 * swap chain images. This function will always return a valid FrameBufferAttachment structure.
 *
 * @param context Contextual Vulkan handles.
 * @return An array of frame buffer attachments each of which points to an image in the swap chain,
 * indexed by swap chain image index.
 */
std::vector<std::unique_ptr<FrameBufferAttachment>>
CreateColorAttachmentsForSwapChain(VulkanContext *context);

/**
 * @brief CreateColorAttachment Creates a color attachment and allocates an image for storing color
 * pixels. The color attachment is default to be samplable by a shader. This function will always
 * return a valid FrameBufferAttachment structure. Any failure occurs during color image/image view
 * allocation will make it fail.
 *
 * @param width The width, in pixels, of the color buffer.
 * @param height The height, in pixels, of the color buffer.
 * @param format The color pixel's format.
 * @param transfer_src If true, allows the color attachment to be a potential transfer source.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBufferAttachment structure.
 */
std::unique_ptr<FrameBufferAttachment> CreateColorAttachment(unsigned width, unsigned height,
                                                             VkFormat format, bool transfer_src,
                                                             VulkanContext *context);

/**
 * @brief CreateDepthAttachment Creates a depth attachment and allocates an image for depth
 * buffering. This function will always return a valid FrameBufferAttachment structure. Any failure
 * occurs during depth image/image view allocation will make it fail.
 *
 * @param width The width, in pixels, of the depth buffer.
 * @param height The height, in pixels, of the depth buffer.
 * @param samplable Sets this to true if the depth image will be sampled by a shader.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBufferAttachment structure.
 */
std::unique_ptr<FrameBufferAttachment>
CreateDepthAttachment(unsigned width, unsigned height, bool samplable, VulkanContext *context);

/**
 * @brief CreateStorageAttachment
 * @param width
 * @param height
 * @param format
 * @param context
 * @return
 */
std::unique_ptr<FrameBufferAttachment>
CreateStorageAttachment(unsigned width, unsigned height, VkFormat format, VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_ATTACHMENT_H
