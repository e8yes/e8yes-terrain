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

#ifndef ISLANDS_RENDERER_UNIFORM_BUFFER_H
#define ISLANDS_RENDERER_UNIFORM_BUFFER_H

#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

/**
 * @brief The UniformBuffer struct It stores data of a uniform variable/block. It will clean up the
 * uniform buffer resource by the end of its lifecycle.
 */
struct UniformBuffer {
    /**
     * @brief UniformBuffer Should be created only by calling CreateUniformBuffer().
     */
    UniformBuffer(unsigned size, VulkanContext *context);
    ~UniformBuffer();

    // A full Vulkan object storing data of the uniform variable/block.
    VkBuffer buffer;

    // The buffer's allocation.
    VmaAllocation allocation;

    // The size of the uniform variable/block
    unsigned size;

    // Contextual Vulkan handles
    VulkanContext *context;
};

/**
 * @brief CreateUniformBuffer Creates a uniform buffer.
 *
 * @param size The size of the uniform buffer in byte.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the UniformBuffer structure.
 */
std::unique_ptr<UniformBuffer> CreateUniformBuffer(unsigned size, VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_UNIFORM_BUFFER_H
