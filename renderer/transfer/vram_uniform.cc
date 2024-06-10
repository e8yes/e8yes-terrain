/**
 * e8yes islands.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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
#include <cstdint>
#include <cstring>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/sampler.h"
#include "renderer/transfer/uniform_promise.h"
#include "renderer/transfer/vram_uniform.h"
#include "resource/buffer_texture.h"

namespace e8 {
namespace {

VkBuffer AllocateUniformBuffer(unsigned size, VmaAllocation *allocation, VulkanContext *context) {
    VkBuffer result;

    VkBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = size;
    vertex_buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    VkResult status = vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info,
                                      &result, allocation,
                                      /*pAllocationInfo=*/nullptr);
    assert(status == VK_SUCCESS);

    return result;
}

void FreeUniformBuffer(VkBuffer buffer, VmaAllocation allocation, VulkanContext *context) {
    vmaDestroyBuffer(context->allocator, buffer, allocation);
}

void UploadUniforms(std::vector<VkWriteDescriptorSet> const &writes, VulkanContext *context) {
    if (writes.empty()) {
        return;
    }

    vkUpdateDescriptorSets(context->device, /*descriptorWriteCount=*/writes.size(), writes.data(),
                           /*descriptorCopyCount=*/0, /*pDescriptorCopies=*/nullptr);
}

void UploadUniformBuffers(std::vector<StagingUniformBuffer> const &buffers,
                          GpuUniformPromise *uniform_promise, VulkanContext *context) {
    std::vector<VkWriteDescriptorSet> writes;
    std::vector<VkDescriptorBufferInfo> buffer_infos_arena;
    writes.reserve(buffers.size());
    buffer_infos_arena.reserve(buffers.size());

    for (auto const &buffer : buffers) {
        VkDescriptorBufferInfo buffer_info{};
        buffer_info.buffer = buffer.buffer;
        buffer_info.offset = 0;
        buffer_info.range = buffer.ActualSize();
        buffer_infos_arena.push_back(std::move(buffer_info));

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.dstSet = uniform_promise->descriptor_set;
        write.dstBinding = buffer.binding;
        write.descriptorCount = 1;
        write.pBufferInfo = &buffer_infos_arena.back();

        writes.push_back(write);
    }

    UploadUniforms(writes, context);
}

std::vector<VkDescriptorImageInfo> ToImageInfo(UniformImagePack const &image_pack) {
    std::vector<VkDescriptorImageInfo> result;
    result.reserve(image_pack.images.size());

    for (auto const &image_view : image_pack.images) {
        VkDescriptorImageInfo image_info{};
        image_info.sampler = image_pack.image_sampler->sampler;
        image_info.imageView = image_view;
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        result.push_back(image_info);
    }

    return result;
}

void UploadUniformImagePacks(std::vector<UniformImagePack> const &image_packs,
                             GpuUniformPromise *uniform_promise, VulkanContext *context) {
    std::vector<VkWriteDescriptorSet> writes;
    std::vector<std::vector<VkDescriptorImageInfo>> image_infos_arena;
    writes.reserve(image_packs.size());
    image_infos_arena.reserve(image_packs.size());

    for (auto const &image_pack : image_packs) {
        image_infos_arena.push_back(ToImageInfo(image_pack));

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.dstSet = uniform_promise->descriptor_set;
        write.dstBinding = image_pack.binding;
        write.descriptorCount = image_infos_arena.back().size();
        write.pImageInfo = image_infos_arena.back().data();

        writes.push_back(write);
    }

    UploadUniforms(writes, context);
}

} // namespace

StagingUniformBuffer::StagingUniformBuffer(unsigned binding, unsigned size, VulkanContext *context)
    : binding(binding), max_size_(size), actual_size_(0), context_(context) {
    buffer = AllocateUniformBuffer(size, &allocation_, context_);
}

StagingUniformBuffer::StagingUniformBuffer(StagingUniformBuffer &&other)
    : binding(other.binding), buffer(VK_NULL_HANDLE), max_size_(other.max_size_), actual_size_(0),
      allocation_(VK_NULL_HANDLE), context_(nullptr) {
    std::swap(buffer, other.buffer);
    std::swap(actual_size_, other.actual_size_);
    std::swap(allocation_, other.allocation_);
    std::swap(context_, other.context_);
}

StagingUniformBuffer::~StagingUniformBuffer() {
    if (buffer == VK_NULL_HANDLE) {
        return;
    }
    FreeUniformBuffer(buffer, allocation_, context_);
}

unsigned StagingUniformBuffer::ActualSize() const { return actual_size_; }

void *StagingUniformBuffer::BeginAccess() {
    assert(buffer != VK_NULL_HANDLE);

    void *data;
    vmaMapMemory(context_->allocator, allocation_, &data);
    return data;
}

void StagingUniformBuffer::EndAccess() {
    assert(buffer != VK_NULL_HANDLE);
    vmaUnmapMemory(context_->allocator, allocation_);
}

StagingUniformImagePack::StagingUniformImagePack(
    unsigned binding, std::vector<StagingTextureBuffer const *> const &images,
    ImageSampler const *image_sampler)
    : binding(binding), images(images), image_sampler(image_sampler) {}

StagingUniformImagePack::~StagingUniformImagePack() = default;

UniformImagePack::UniformImagePack(unsigned binding, std::vector<VkImageView> const &images,
                                   ImageSampler const *image_sampler)
    : binding(binding), images(images), image_sampler(image_sampler) {}

UniformImagePack::~UniformImagePack() = default;

UniformVramTransfer::UniformVramTransfer(VulkanContext *context)
    : allocator_(GpuUniformPromiseAllocator(context)), context_(context) {}

UniformVramTransfer::~UniformVramTransfer() = default;

void UniformVramTransfer::Upload(TransferId const &transfer_id,
                                 std::vector<StagingUniformBuffer> const &buffers,
                                 std::vector<UniformImagePack> const &image_packs,
                                 VkDescriptorSetLayout layout) {
    auto promise_it = promises_.find(transfer_id);
    if (promise_it == promises_.end()) {
        std::unique_ptr<GpuUniformPromise> promise = allocator_.Allocate(layout);
        promise_it = promises_.insert(std::make_pair(transfer_id, std::move(promise))).first;
    }

    auto &[_, uniform_promise] = *promise_it;
    UploadUniformBuffers(buffers, uniform_promise.get(), context_);
    UploadUniformImagePacks(image_packs, uniform_promise.get(), context_);
}

GpuUniformPromise *UniformVramTransfer::Find(TransferId const &transfer_id) {
    auto promise_it = promises_.find(transfer_id);
    if (promise_it == promises_.end()) {
        return nullptr;
    }

    return promise_it->second.get();
}

void UniformVramTransfer::Evict(TransferId const &transfer_id) { promises_.erase(transfer_id); }

} // namespace e8
