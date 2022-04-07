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
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

FrameBufferAttachment::FrameBufferAttachment(bool swap_chain_image, VulkanContext *context)
    : swap_chain_image(swap_chain_image), allocation(VK_NULL_HANDLE), desc{}, context_(context) {}

FrameBufferAttachment::~FrameBufferAttachment() {
    if (!swap_chain_image) {
        vkDestroyImageView(context_->device, view, /*pAllocator=*/nullptr);
        vmaDestroyImage(context_->allocator, image, allocation);
    }
}

std::vector<std::unique_ptr<FrameBufferAttachment>>
CreateColorAttachmentsForSwapChain(VulkanContext *context) {
    std::vector<std::unique_ptr<FrameBufferAttachment>> infos;

    for (unsigned i = 0; i < context->swap_chain_images.size(); ++i) {
        auto info = std::make_unique<FrameBufferAttachment>(/*swap_chain_image=*/true, context);

        info->image = context->swap_chain_images[i];
        info->view = context->swap_chain_image_views[i];

        info->desc.flags = 0;
        info->desc.format = context->swap_chain_image_format.format;
        info->desc.samples = VK_SAMPLE_COUNT_1_BIT;
        info->desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        info->desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        info->desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        info->desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        info->desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        info->desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        infos.emplace_back(std::move(info));
    }

    return infos;
}

std::unique_ptr<FrameBufferAttachment>
CreateColorAttachment(unsigned width, unsigned height, VkFormat format, VulkanContext *context) {
    auto info = std::make_unique<FrameBufferAttachment>(/*swap_chain_image=*/false, context);

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = format;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags =
        VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    assert(VK_SUCCESS == vmaCreateImage(context->allocator, &image_info, &allocation_create_info,
                                        &info->image, &info->allocation,
                                        /*pAllocationInfo=*/nullptr));

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.image = info->image;
    view_info.format = format;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    assert(VK_SUCCESS == vkCreateImageView(context->device, &view_info,
                                           /*pAllocator=*/nullptr, &info->view));

    info->desc.flags = 0;
    info->desc.format = format;
    info->desc.samples = VK_SAMPLE_COUNT_1_BIT;
    info->desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    info->desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    info->desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    info->desc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    return info;
}

std::unique_ptr<FrameBufferAttachment>
CreateDepthAttachment(unsigned width, unsigned height, bool samplable, VulkanContext *context) {
    auto info = std::make_unique<FrameBufferAttachment>(/*swap_chain_image=*/false, context);

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = VK_FORMAT_D32_SFLOAT;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (samplable) {
        image_info.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }

    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags =
        VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    assert(VK_SUCCESS == vmaCreateImage(context->allocator, &image_info, &allocation_create_info,
                                        &info->image, &info->allocation,
                                        /*pAllocationInfo=*/nullptr));

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.image = info->image;
    view_info.format = VK_FORMAT_D32_SFLOAT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    assert(VK_SUCCESS == vkCreateImageView(context->device, &view_info,
                                           /*pAllocator=*/nullptr, &info->view));

    info->desc.flags = 0;
    info->desc.format = VK_FORMAT_D32_SFLOAT;
    info->desc.samples = VK_SAMPLE_COUNT_1_BIT;
    info->desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    info->desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    info->desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (samplable) {
        info->desc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    } else {
        info->desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    return info;
}

} // namespace e8
