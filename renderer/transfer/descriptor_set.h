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

#ifndef ISLANDS_RENDERER_DESCRIPTOR_SET_H
#define ISLANDS_RENDERER_DESCRIPTOR_SET_H

#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {
namespace descriptor_set_internal {

struct DescriptorPool;

} // namespace descriptor_set_internal

/**
 * @brief The DescriptorType enum The descriptor types.
 */
enum DescriptorType {
    DT_UNIFORM_BUFFER,
    DT_COMBINED_IMAGE_SAMPLER,
    DT_DESCRIPTOR_TYPE_COUNT,
};

/**
 * @brief The DescriptorSetType enum Descriptor set bind points catagorized by binding frequnecy.
 */
enum DescriptorSetType {
    DST_PER_FRAME,
    DST_PER_PASS,
    DST_PER_DRAWABLE,
    DST_DESCRIPTOR_SET_TYPE_COUNT,
};

/**
 * @brief The DescriptorSet struct An allocated descriptor set instance. It automatically cleans up
 * the resource by the end of its lifecycle.
 */
struct DescriptorSet {
    /**
     * @brief DescriptorSets Should be created only by calling DescriptorSetAllocator::Allocate() or
     * MaterialDescriptorSetAllocator::DescriptorSetFor().
     */
    DescriptorSet(VkDescriptorSet descriptor_set, descriptor_set_internal::DescriptorPool *pool);
    DescriptorSet(DescriptorSet const &) = delete;
    DescriptorSet(DescriptorSet &&other);
    ~DescriptorSet();

    DescriptorSet &operator=(DescriptorSet &&other);

    // A fully Vulkan object which represents a descriptor set instance.
    VkDescriptorSet descriptor_set;

    // The pool the descriptor was allocated from.
    descriptor_set_internal::DescriptorPool *pool;
};

/**
 * @brief The DescriptorSetAllocator class A generic descriptor set allocator which allows arbitrary
 * number of descriptor set allocations.
 */
class DescriptorSetAllocator {
  public:
    explicit DescriptorSetAllocator(VulkanContext *context);
    DescriptorSetAllocator(DescriptorSetAllocator const &) = delete;
    ~DescriptorSetAllocator();

    /**
     * @brief Allocate Allocates a descriptor set of the specified type and layout. This function
     * always returns a valid descriptor set unless the device/host memory is exhausted.
     */
    std::unique_ptr<DescriptorSet> Allocate(DescriptorType type, VkDescriptorSetLayout layout);

  private:
    struct DescriptorSetAllocatorImpl;
    std::unique_ptr<DescriptorSetAllocatorImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DESCRIPTOR_SET_H
