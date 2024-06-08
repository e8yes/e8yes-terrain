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

#ifndef ISLANDS_RENDERER_UNIFORM_PROMISE_H
#define ISLANDS_RENDERER_UNIFORM_PROMISE_H

#include <deque>
#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {
namespace uniform_promise_internal {

struct UniformPromisePool;

} // namespace uniform_promise_internal

/**
 * @brief The GpuUniformPromise struct An allocated GPU uniform promise instance. It represents the
 * location to which uniform buffer data will be uploaded. It automatically cleans up the
 * resource by the end of its lifecycle.
 */
struct GpuUniformPromise {
    /**
     * @brief GpuUniformPromise Should be created only by calling
     * GpuUniformPromiseAllocator::Allocate().
     */
    GpuUniformPromise(VkDescriptorSet descriptor_set,
                      std::shared_ptr<uniform_promise_internal::UniformPromisePool> const &pool);
    GpuUniformPromise(GpuUniformPromise const &) = delete;
    GpuUniformPromise(GpuUniformPromise &&other);
    ~GpuUniformPromise();

    GpuUniformPromise &operator=(GpuUniformPromise &&other);

    // A fully Vulkan object which represents a descriptor set instance.
    VkDescriptorSet descriptor_set;

    // The pool the descriptor was allocated from.
    std::shared_ptr<uniform_promise_internal::UniformPromisePool> pool;
};

/**
 * @brief The GpuUniformPromiseAllocator class A generic GPU uniform promise allocator.
 */
class GpuUniformPromiseAllocator {
  public:
    /**
     * @brief GpuUniformPromiseAllocator
     * @param layout_cateogry
     * @param context
     */
    GpuUniformPromiseAllocator(VulkanContext *context);
    GpuUniformPromiseAllocator(GpuUniformPromiseAllocator const &) = delete;
    ~GpuUniformPromiseAllocator();

    /**
     * @brief Allocate Allocates a GPU uniform promise of the specified layout. This function always
     * returns a valid promise unless the device/host memory is exhausted.
     */
    std::unique_ptr<GpuUniformPromise> Allocate(VkDescriptorSetLayout layout);

  private:
    using DescriptorPools =
        std::deque<std::shared_ptr<uniform_promise_internal::UniformPromisePool>>;

    DescriptorPools pools_;
    VulkanContext *context_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_UNIFORM_PROMISE_H
