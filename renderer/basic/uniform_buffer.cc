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
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/uniform_buffer.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

UniformBuffer::UniformBuffer(unsigned size, VulkanContext *context) : size(size), context(context) {
    VkBufferCreateInfo uniform_buffer_info{};
    uniform_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    uniform_buffer_info.size = size;
    uniform_buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    assert(VK_SUCCESS == vmaCreateBuffer(context->allocator, &uniform_buffer_info, &allocation_info,
                                         &buffer, &allocation, /*pAllocatorInfo=*/nullptr));
}

UniformBuffer::~UniformBuffer() { vmaDestroyBuffer(context->allocator, buffer, allocation); }

std::unique_ptr<UniformBuffer> CreateUniformBuffer(unsigned size, VulkanContext *context) {
    return std::make_unique<UniformBuffer>(size, context);
}

} // namespace e8
