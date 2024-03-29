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
#include <array>
#include <cassert>
#include <deque>
#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/uniform_buffer.h"
#include "renderer/transfer/descriptor_set.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {
namespace {

unsigned const kDescriptorPoolCapacity = 1000;

} // namespace

namespace descriptor_set_internal {

struct DescriptorPool {
    DescriptorPool(unsigned capacity, DescriptorType type, VulkanContext *context);
    DescriptorPool(DescriptorPool const &) = delete;
    DescriptorPool(DescriptorPool &&other);
    ~DescriptorPool();

    DescriptorPool &operator=(DescriptorPool &&other);
    void Free();

    VkDescriptorPool pool;
    VulkanContext *context;
};

DescriptorPool::DescriptorPool(unsigned capacity, DescriptorType type, VulkanContext *context)
    : context(context) {
    VkDescriptorPoolSize pool_size{};
    switch (type) {
    case DT_UNIFORM_BUFFER: {
        pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        break;
    }
    case DT_COMBINED_IMAGE_SAMPLER: {
        pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        break;
    }
    default: {
        assert(false);
    }
    }
    pool_size.descriptorCount = capacity;

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.pPoolSizes = &pool_size;
    pool_info.poolSizeCount = 1;
    pool_info.maxSets = capacity;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    assert(VK_SUCCESS == vkCreateDescriptorPool(context->device, &pool_info,
                                                /*pAllocator=*/nullptr, &pool));
}

DescriptorPool::DescriptorPool(DescriptorPool &&other) : pool(VK_NULL_HANDLE) {
    *this = std::move(other);
}

DescriptorPool::~DescriptorPool() { this->Free(); }

DescriptorPool &DescriptorPool::operator=(DescriptorPool &&other) {
    this->Free();

    context = other.context;
    pool = other.pool;
    other.pool = VK_NULL_HANDLE;

    return *this;
}

void DescriptorPool::Free() {
    if (pool == VK_NULL_HANDLE) {
        return;
    }

    vkDestroyDescriptorPool(context->device, pool, /*pAllocator=*/nullptr);
    pool = VK_NULL_HANDLE;
}

class DescriptorPools {
  public:
    DescriptorPools() = default;
    ~DescriptorPools() = default;

    void Init(DescriptorType type, VulkanContext *context) {
        type_ = type;
        context_ = context;
    }

    std::unique_ptr<DescriptorSet> Allocate(VkDescriptorSetLayout layout) {
        std::unique_ptr<DescriptorSet> result = this->AllocateFromExistingPools(layout);
        if (result != nullptr) {
            return result;
        }

        pools_.push_front(
            std::make_unique<DescriptorPool>(kDescriptorPoolCapacity, type_, context_));
        result = this->AllocateFromExistingPools(layout);
        assert(result != nullptr);

        return result;
    }

  private:
    std::unique_ptr<DescriptorSet> AllocateFromExistingPools(VkDescriptorSetLayout layout) {
        if (pools_.empty()) {
            return nullptr;
        }

        VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
        std::deque<std::shared_ptr<DescriptorPool>>::iterator spacious_pool_it;

        for (spacious_pool_it = pools_.begin(); spacious_pool_it != pools_.end();
             ++spacious_pool_it) {
            VkDescriptorSetAllocateInfo desc_set_alloc_info{};
            desc_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            desc_set_alloc_info.descriptorPool = spacious_pool_it->get()->pool;
            desc_set_alloc_info.pSetLayouts = &layout;
            desc_set_alloc_info.descriptorSetCount = 1;

            VkResult result =
                vkAllocateDescriptorSets(context_->device, &desc_set_alloc_info, &descriptor_set);

            if (result == VK_SUCCESS) {
                break;
            }

            assert(result == VK_ERROR_OUT_OF_POOL_MEMORY);
        }

        if (descriptor_set == VK_NULL_HANDLE) {
            // All allocation attempts failed.
            return nullptr;
        }

        if (spacious_pool_it != pools_.begin()) {
            // Moves the spacious pool to the front of the queue to make next allocation faster.
            std::swap(*pools_.begin(), *spacious_pool_it);
        }

        return std::make_unique<DescriptorSet>(descriptor_set, *spacious_pool_it);
    }

    DescriptorType type_;
    VulkanContext *context_;
    std::deque<std::shared_ptr<DescriptorPool>> pools_;
};

} // namespace descriptor_set_internal

DescriptorSet::DescriptorSet(VkDescriptorSet descriptor_set,
                             std::shared_ptr<descriptor_set_internal::DescriptorPool> const &pool)
    : descriptor_set(descriptor_set), pool(pool) {}

DescriptorSet::DescriptorSet(DescriptorSet &&other)
    : descriptor_set(VK_NULL_HANDLE), pool(nullptr) {
    *this = std::move(other);
}

DescriptorSet::~DescriptorSet() {
    if (descriptor_set == VK_NULL_HANDLE) {
        return;
    }

    vkFreeDescriptorSets(pool->context->device, pool->pool, /*descriptorSetCount=*/1,
                         &descriptor_set);
    descriptor_set = VK_NULL_HANDLE;
}

DescriptorSet &DescriptorSet::operator=(DescriptorSet &&other) {
    std::swap(descriptor_set, other.descriptor_set);
    std::swap(pool, other.pool);
    return *this;
}

struct DescriptorSetAllocator::DescriptorSetAllocatorImpl {
    DescriptorSetAllocatorImpl(VulkanContext *context);
    ~DescriptorSetAllocatorImpl();

    std::array<descriptor_set_internal::DescriptorPools, DT_DESCRIPTOR_TYPE_COUNT> type_to_pools;
    VulkanContext *context;
};

DescriptorSetAllocator::DescriptorSetAllocatorImpl::DescriptorSetAllocatorImpl(
    VulkanContext *context)
    : context(context) {
    for (unsigned i = 0; i < DT_DESCRIPTOR_TYPE_COUNT; ++i) {
        type_to_pools[i].Init(static_cast<DescriptorType>(i), context);
    }
}

DescriptorSetAllocator::DescriptorSetAllocatorImpl::~DescriptorSetAllocatorImpl() {}

DescriptorSetAllocator::DescriptorSetAllocator(VulkanContext *context)
    : pimpl_(std::make_unique<DescriptorSetAllocatorImpl>(context)) {}

DescriptorSetAllocator::~DescriptorSetAllocator() {}

std::unique_ptr<DescriptorSet> DescriptorSetAllocator::Allocate(DescriptorType type,
                                                                VkDescriptorSetLayout layout) {
    return pimpl_->type_to_pools[type].Allocate(layout);
}

void WriteUniformBufferDescriptor(void *data, UniformBuffer const &uniform_buffer,
                                  DescriptorSet const &descriptor_set, unsigned binding,
                                  VulkanContext *context) {
    // Updates the host buffer.
    void *staging_buffer;
    vmaMapMemory(context->allocator, uniform_buffer.allocation, &staging_buffer);
    memcpy(staging_buffer, data, uniform_buffer.size);
    vmaUnmapMemory(context->allocator, uniform_buffer.allocation);

    // Writes the updated buffer to the descriptor.
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = uniform_buffer.buffer;
    buffer_info.offset = 0;
    buffer_info.range = uniform_buffer.size;

    VkWriteDescriptorSet write_descriptor{};
    write_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write_descriptor.dstSet = descriptor_set.descriptor_set;
    write_descriptor.dstBinding = binding;
    write_descriptor.descriptorCount = 1;
    write_descriptor.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(context->device, /*descriptorWriteCount=*/1, &write_descriptor,
                           /*descriptorCopyCount=*/0, /*pDescriptorCopies=*/nullptr);
}

void WriteImageDescriptor(VkImageView image_view, ImageSampler const &image_sampler,
                          DescriptorSet const &descriptor_set, unsigned binding,
                          VulkanContext *context) {
    VkDescriptorImageInfo image_info{};
    image_info.sampler = image_sampler.sampler;
    image_info.imageView = image_view;
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet write_descriptor{};
    write_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write_descriptor.dstSet = descriptor_set.descriptor_set;
    write_descriptor.dstBinding = binding;
    write_descriptor.descriptorCount = 1;
    write_descriptor.pImageInfo = &image_info;

    vkUpdateDescriptorSets(context->device, /*descriptorWriteCount=*/1, &write_descriptor,
                           /*descriptorCopyCount=*/0, /*pDescriptorCopies=*/nullptr);
}

} // namespace e8
