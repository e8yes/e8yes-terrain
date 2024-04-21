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
#include <deque>
#include <memory>
#include <utility>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/transfer/uniform_promise.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {
namespace {

unsigned const kUniformPromisePoolCapacity = 1000;
unsigned const kUniformBufferCount = 2000;
unsigned const kImageCount = 500;

} // namespace

namespace uniform_promise_internal {

struct UniformPromisePool {
    UniformPromisePool(VulkanContext *context) : context(context) {
        std::vector<VkDescriptorPoolSize> pool_sizes{
            VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                 .descriptorCount = kUniformBufferCount},
            VkDescriptorPoolSize{
                .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = kImageCount,
            }};

        VkDescriptorPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.pPoolSizes = pool_sizes.data();
        pool_info.poolSizeCount = pool_sizes.size();
        pool_info.maxSets = kUniformPromisePoolCapacity;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        assert(VK_SUCCESS == vkCreateDescriptorPool(context->device, &pool_info,
                                                    /*pAllocator=*/nullptr, &pool));
    }

    UniformPromisePool(UniformPromisePool const &) = delete;

    UniformPromisePool(UniformPromisePool &&other) : pool(VK_NULL_HANDLE) {
        *this = std::move(other);
    }

    ~UniformPromisePool() { this->Free(); }

    UniformPromisePool &operator=(UniformPromisePool &&other) {
        this->Free();

        context = other.context;
        pool = other.pool;
        other.pool = VK_NULL_HANDLE;

        return *this;
    }

    void Free() {
        if (pool == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyDescriptorPool(context->device, pool, /*pAllocator=*/nullptr);
        pool = VK_NULL_HANDLE;
    }

    VkDescriptorPool pool;
    VulkanContext *context;
};

std::unique_ptr<GpuUniformPromise>
AllocateFromPools(std::deque<std::shared_ptr<uniform_promise_internal::UniformPromisePool>> *pools,
                  VkDescriptorSetLayout layout, VulkanContext *context) {
    if (pools->empty()) {
        return nullptr;
    }

    VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
    std::deque<std::shared_ptr<UniformPromisePool>>::iterator spacious_pool_it;

    for (spacious_pool_it = pools->begin(); spacious_pool_it != pools->end(); ++spacious_pool_it) {
        VkDescriptorSetAllocateInfo desc_set_alloc_info{};
        desc_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        desc_set_alloc_info.descriptorPool = spacious_pool_it->get()->pool;
        desc_set_alloc_info.pSetLayouts = &layout;
        desc_set_alloc_info.descriptorSetCount = 1;

        VkResult result =
            vkAllocateDescriptorSets(context->device, &desc_set_alloc_info, &descriptor_set);

        if (result == VK_SUCCESS) {
            break;
        }

        assert(result == VK_ERROR_OUT_OF_POOL_MEMORY);
    }

    if (descriptor_set == VK_NULL_HANDLE) {
        // All allocation attempts failed.
        return nullptr;
    }

    if (spacious_pool_it != pools->begin()) {
        // Moves the spacious pool to the front of the queue to make next allocation faster.
        std::swap(*pools->begin(), *spacious_pool_it);
    }

    return std::make_unique<GpuUniformPromise>(descriptor_set, *spacious_pool_it);
}

} // namespace uniform_promise_internal

GpuUniformPromise::GpuUniformPromise(
    VkDescriptorSet descriptor_set,
    std::shared_ptr<uniform_promise_internal::UniformPromisePool> const &pool)
    : descriptor_set(descriptor_set), pool(pool) {}

GpuUniformPromise::GpuUniformPromise(GpuUniformPromise &&other)
    : descriptor_set(VK_NULL_HANDLE), pool(nullptr) {
    *this = std::move(other);
}

GpuUniformPromise::~GpuUniformPromise() {
    if (descriptor_set == VK_NULL_HANDLE) {
        return;
    }

    vkFreeDescriptorSets(pool->context->device, pool->pool, /*descriptorSetCount=*/1,
                         &descriptor_set);
    descriptor_set = VK_NULL_HANDLE;
}

GpuUniformPromise &GpuUniformPromise::operator=(GpuUniformPromise &&other) {
    std::swap(descriptor_set, other.descriptor_set);
    std::swap(pool, other.pool);
    return *this;
}

GpuUniformPromiseAllocator::GpuUniformPromiseAllocator(VulkanContext *context)
    : context_(context) {}

GpuUniformPromiseAllocator::~GpuUniformPromiseAllocator() = default;

std::unique_ptr<GpuUniformPromise>
GpuUniformPromiseAllocator::Allocate(VkDescriptorSetLayout layout) {
    std::unique_ptr<GpuUniformPromise> result = AllocateFromPools(&pools_, layout, context_);
    if (result != nullptr) {
        return result;
    }

    // Old pools are used up.
    pools_.push_front(std::make_shared<uniform_promise_internal::UniformPromisePool>(context_));
    result = AllocateFromPools(&pools_, layout, context_);
    assert(result != nullptr);

    return result;
}

} // namespace e8
