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
#include <chrono>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/transfer/vram.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {
namespace {

std::chrono::nanoseconds const kUploadTimeout = std::chrono::seconds(10);

} // namespace

VramTransfer::GpuBuffer::GpuBuffer()
    : buffer(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), context(nullptr) {}

VramTransfer::GpuBuffer::GpuBuffer(GpuBuffer &&other) : GpuBuffer() { *this = std::move(other); }

VramTransfer::GpuBuffer::~GpuBuffer() { this->Free(); }

VramTransfer::GpuBuffer &VramTransfer::GpuBuffer::operator=(GpuBuffer &&other) {
    this->Free();

    buffer = other.buffer;
    allocation = other.allocation;
    context = other.context;

    other.buffer = VK_NULL_HANDLE;
    other.allocation = VK_NULL_HANDLE;
    other.context = nullptr;

    return *this;
}

void VramTransfer::GpuBuffer::Allocate(VkBufferUsageFlags usage, unsigned size,
                                       VulkanContext *context) {
    this->context = context;

    this->Free();

    VkBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = size;
    vertex_buffer_info.usage = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info, &buffer, &allocation,
                    /*pAllocationInfo=*/nullptr);
}

void VramTransfer::GpuBuffer::Free() {
    if (this->Valid()) {
        assert(context != nullptr);
        vmaDestroyBuffer(context->allocator, buffer, allocation);
    }
}

bool VramTransfer::GpuBuffer::Valid() const { return buffer != VK_NULL_HANDLE; }

VramTransfer::GpuTexture::GpuTexture()
    : image(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), view(VK_NULL_HANDLE), context(nullptr) {}

VramTransfer::GpuTexture::GpuTexture(GpuTexture &&other) : GpuTexture() {
    *this = std::move(other);
}

VramTransfer::GpuTexture::~GpuTexture() { this->Free(); }

VramTransfer::GpuTexture &VramTransfer::GpuTexture::operator=(VramTransfer::GpuTexture &&other) {
    this->Free();

    image = other.image;
    allocation = other.allocation;
    view = other.view;
    context = other.context;

    other.image = VK_NULL_HANDLE;
    other.allocation = VK_NULL_HANDLE;
    other.view = VK_NULL_HANDLE;
    other.context = nullptr;

    return *this;
}

void VramTransfer::GpuTexture::Allocate(unsigned width, unsigned height, VkFormat image_format,
                                        unsigned mip_level_count, VulkanContext *context) {
    this->Free();

    this->context = context;

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = image_format;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = mip_level_count;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                       VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    assert(VK_SUCCESS == vmaCreateImage(context->allocator, &image_info, &allocation_create_info,
                                        &image, &allocation,
                                        /*pAllocationInfo=*/nullptr));

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.image = image;
    view_info.format = image_format;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = mip_level_count;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    assert(VK_SUCCESS ==
           vkCreateImageView(context->device, &view_info, /*pAllocator=*/nullptr, &view));
}

void VramTransfer::GpuTexture::Free() {
    if (!this->Valid()) {
        return;
    }

    vkDestroyImageView(context->device, view, /*pAllocator=*/nullptr);
    vmaDestroyImage(context->allocator, image, allocation);

    image = VK_NULL_HANDLE;
    allocation = VK_NULL_HANDLE;
    view = VK_NULL_HANDLE;
    context = nullptr;
}

bool VramTransfer::GpuTexture::Valid() const { return image != VK_NULL_HANDLE; }

VramTransfer::VramTransfer(VulkanContext *context) : context_(context) {
    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    assert(VK_SUCCESS ==
           vkCreateFence(context->device, &fence_info, /*pAllocator=*/nullptr, &fence_));
}

VramTransfer::~VramTransfer() { vkDestroyFence(context_->device, fence_, /*pAllocator=*/nullptr); }

VkCommandBuffer VramTransfer::BeginUpload() {
    // Allocates a new command buffer for storing buffer upload commands.
    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context_->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmds;
    assert(VK_SUCCESS == vkAllocateCommandBuffers(context_->device, &cmds_allocation_info, &cmds));

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(cmds, &command_buffer_begin_info));

    return cmds;
}

void VramTransfer::EndUpload(VkCommandBuffer cmds) {
    assert(VK_SUCCESS == vkEndCommandBuffer(cmds));

    // Starts copying vertex and index buffers.
    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.pCommandBuffers = &cmds;
    submit.commandBufferCount = 1;

    assert(VK_SUCCESS ==
           vkQueueSubmit(context_->graphics_queue, /*submitCount=*/1, &submit, fence_));

    // Waits for the copying to complete and releases the command buffer.
    assert(VK_SUCCESS == vkWaitForFences(context_->device, /*fenceCount=*/1, &fence_,
                                         /*waitAll=*/VK_TRUE, kUploadTimeout.count()));
    assert(VK_SUCCESS == vkResetFences(context_->device, /*fenceCount=*/1, &fence_));

    vkFreeCommandBuffers(context_->device, context_->command_pool, /*commandBufferCount=*/1, &cmds);
}

} // namespace e8
