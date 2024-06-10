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
#include <utility>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "resource/buffer.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

StagingBuffer::StagingBuffer()
    : buffer(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), context(nullptr) {}

StagingBuffer::StagingBuffer(StagingBuffer &&other) : StagingBuffer() {
    std::swap(buffer, other.buffer);
    std::swap(allocation, other.allocation);
    std::swap(context, other.context);
}

StagingBuffer::~StagingBuffer() { this->Free(); }

StagingBuffer &StagingBuffer::operator=(StagingBuffer &&other) {
    this->Free();

    buffer = other.buffer;
    allocation = other.allocation;
    context = other.context;

    other.buffer = VK_NULL_HANDLE;
    other.allocation = VK_NULL_HANDLE;
    other.context = nullptr;

    return *this;
}

void StagingBuffer::Allocate(unsigned size, VulkanContext *context) {
    this->context = context;
    this->Free();

    VkBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = size;
    vertex_buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info, &buffer, &allocation,
                    /*pAllocationInfo=*/nullptr);
}

void StagingBuffer::Free() {
    if (!this->Valid()) {
        return;
    }

    vmaDestroyBuffer(context->allocator, buffer, allocation);
    buffer = VK_NULL_HANDLE;
    allocation = VK_NULL_HANDLE;
    context = nullptr;
}

void *StagingBuffer::BeginAccess() {
    assert(this->Valid());

    void *data;
    vmaMapMemory(context->allocator, allocation, &data);
    return data;
}

void const *StagingBuffer::BeginAccess() const {
    assert(this->Valid());

    void *data;
    vmaMapMemory(context->allocator, allocation, &data);
    return data;
}

void StagingBuffer::EndAccess() const {
    assert(this->Valid());

    vmaUnmapMemory(context->allocator, allocation);
}

bool StagingBuffer::Valid() const { return buffer != VK_NULL_HANDLE; }

} // namespace e8
