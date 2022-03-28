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

#ifndef ISLANDS_RESOURCE_BUFFER_H
#define ISLANDS_RESOURCE_BUFFER_H

#include <vulkan/vulkan.h>

#include "common/device.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

/**
 * @brief The StagingBuffer class Represents a generic host-only Vulkan buffer.
 */
class StagingBuffer {
  public:
    StagingBuffer();
    StagingBuffer(StagingBuffer const &) = delete;
    StagingBuffer(StagingBuffer &&other);
    ~StagingBuffer();

    StagingBuffer &operator=(StagingBuffer &&other);

    /**
     * @brief Allocate
     * @param size
     * @param context
     */
    void Allocate(unsigned size, VulkanContext *context);

    /**
     * @brief Free
     */
    void Free();

    /**
     * @brief BeginAccess
     * @return
     */
    void const *BeginAccess() const;

    /**
     * @brief BeginAccess
     * @return
     */
    void *BeginAccess();

    /**
     * @brief EndAccess
     */
    void EndAccess() const;

    /**
     * @brief Valid Indicates if the buffer is valid.
     */
    bool Valid() const;

  public:
    // The allocated buffer object.
    VkBuffer buffer;

    // Information about the buffer allocation.
    VmaAllocation allocation;

    // Contextual Vulkan handles.
    VulkanContext *context;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_BUFFER_H
