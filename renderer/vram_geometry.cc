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
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/cache.h"
#include "common/device.h"
#include "content/proto/camera.pb.h"
#include "renderer/vram_geometry.h"
#include "resource/buffer_index.h"
#include "resource/buffer_vertex.h"
#include "resource/geometry.h"

namespace e8 {
namespace {

std::chrono::nanoseconds const kUploadTimeout = std::chrono::seconds(10);

void UploadVertexBuffer(Geometry const *geometry, uint64_t old_buffer_size,
                        uint64_t new_buffer_size, VulkanContext *context,
                        GeometryVramTransfer::BufferUploadResult *vertex_buffer_object,
                        VkCommandBuffer cmds) {
    // Ensures the geometry's vertex buffer was allocated under the same context.
    assert(geometry->vertices.Valid());
    assert(geometry->vertices.context == context);

    if (new_buffer_size != old_buffer_size) {
        vertex_buffer_object->Allocate(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, new_buffer_size, context);
    }

    VkBufferCopy region{};
    region.size = geometry->vertices.BufferSize();

    vkCmdCopyBuffer(cmds, geometry->vertices.buffer, vertex_buffer_object->buffer,
                    /*regionCount=*/1, &region);
}

void UploadIndexBuffer(Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
                       VulkanContext *context,
                       GeometryVramTransfer::BufferUploadResult *index_buffer_object,
                       VkCommandBuffer cmds) {
    // Ensures the geometry's index buffer was allocated under the same context.
    assert(geometry->indices.Valid());
    assert(geometry->indices.context == context);

    if (new_buffer_size != old_buffer_size) {
        index_buffer_object->Allocate(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, new_buffer_size, context);
    }

    VkBufferCopy region{};
    region.size = geometry->indices.BufferSize();

    vkCmdCopyBuffer(cmds, geometry->indices.buffer, index_buffer_object->buffer, /*regionCount=*/1,
                    &region);
}

} // namespace

struct GeometryVramTransfer::GeometryVramTransferImpl {
    GeometryVramTransferImpl(VulkanContext *context);
    ~GeometryVramTransferImpl();

    VkCommandBuffer BeginUpload();
    void EndUpload(VkCommandBuffer cmds);
    void Upload(Geometry const *geometry, VkCommandBuffer cmds);

    DeviceCache<Geometry const *, BufferUploadResult> vertex_cache;
    DeviceCache<Geometry const *, BufferUploadResult> index_cache;
    VkFence fence;
    VulkanContext *context;
};

GeometryVramTransfer::GeometryVramTransferImpl::GeometryVramTransferImpl(VulkanContext *context)
    : vertex_cache(&context->geometry_vram_usage), index_cache(&context->geometry_vram_usage),
      context(context) {
    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    assert(VK_SUCCESS ==
           vkCreateFence(context->device, &fence_info, /*pAllocator=*/nullptr, &fence));
}

GeometryVramTransfer::GeometryVramTransferImpl::~GeometryVramTransferImpl() {
    vkDestroyFence(context->device, fence, /*pAllocator=*/nullptr);
}

VkCommandBuffer GeometryVramTransfer::GeometryVramTransferImpl::BeginUpload() {
    // Allocates a new command buffer for storing buffer upload commands.
    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmds;
    assert(VK_SUCCESS == vkAllocateCommandBuffers(context->device, &cmds_allocation_info, &cmds));

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(cmds, &command_buffer_begin_info));

    return cmds;
}

void GeometryVramTransfer::GeometryVramTransferImpl::EndUpload(VkCommandBuffer cmds) {
    assert(VK_SUCCESS == vkEndCommandBuffer(cmds));

    // Starts copying vertex and index buffers.
    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.pCommandBuffers = &cmds;
    submit.commandBufferCount = 1;

    assert(VK_SUCCESS == vkQueueSubmit(context->graphics_queue, /*submitCount=*/1, &submit, fence));

    // Waits for the copying to complete and releases the command buffer.
    assert(VK_SUCCESS == vkWaitForFences(context->device, /*fenceCount=*/1, &fence,
                                         /*waitAll=*/VK_TRUE, kUploadTimeout.count()));
    assert(VK_SUCCESS == vkResetFences(context->device, /*fenceCount=*/1, &fence));

    vkFreeCommandBuffers(context->device, context->command_pool, /*commandBufferCount=*/1, &cmds);
}

void GeometryVramTransfer::GeometryVramTransferImpl::Upload(Geometry const *geometry,
                                                            VkCommandBuffer cmds) {
    vertex_cache.Upload(
        geometry,
        /*override_old_upload=*/geometry->rigidity == GeometryProto::DEFORMABLE ||
            geometry->rigidity == GeometryProto::TEARABLE,
        /*object_size_fn=*/[](Geometry const *geo) { return geo->vertices.BufferSize(); },
        /*upload_fn=*/
        [this, cmds](Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
                     GeometryVramTransfer::BufferUploadResult *vertex_buffer_object) {
            UploadVertexBuffer(geometry, old_buffer_size, new_buffer_size, context,
                               vertex_buffer_object, cmds);
        });

    index_cache.Upload(
        geometry, /*override_old_upload=*/geometry->rigidity == GeometryProto::TEARABLE,
        /*object_size_fn=*/[](Geometry const *geo) { return geo->indices.BufferSize(); },
        /*upload_fn=*/
        [this, cmds](Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
                     GeometryVramTransfer::BufferUploadResult *vertex_buffer_object) {
            UploadIndexBuffer(geometry, old_buffer_size, new_buffer_size, context,
                              vertex_buffer_object, cmds);
        });
}

GeometryVramTransfer::BufferUploadResult::BufferUploadResult()
    : buffer(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), size(0), context(nullptr) {}

GeometryVramTransfer::BufferUploadResult::~BufferUploadResult() { this->Free(); }

void GeometryVramTransfer::BufferUploadResult::Allocate(VkBufferUsageFlags usage, unsigned size,
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

void GeometryVramTransfer::BufferUploadResult::Free() {
    if (this->Valid()) {
        assert(context != nullptr);
        vmaDestroyBuffer(context->allocator, buffer, allocation);
    }
}

bool GeometryVramTransfer::BufferUploadResult::Valid() const { return buffer != VK_NULL_HANDLE; }

GeometryVramTransfer::GeometryVramTransfer(VulkanContext *context)
    : pimpl_(std::make_unique<GeometryVramTransfer::GeometryVramTransferImpl>(context)) {}

GeometryVramTransfer::~GeometryVramTransfer() {}

GeometryVramTransfer::GpuGeometry::GpuGeometry()
    : vertex_buffer(nullptr), index_buffer(nullptr),
      index_element_type(VkIndexType::VK_INDEX_TYPE_MAX_ENUM) {}

GeometryVramTransfer::GpuGeometry::~GpuGeometry() {}

bool GeometryVramTransfer::GpuGeometry::Valid() const {
    return vertex_buffer != nullptr && vertex_buffer->Valid() && index_buffer != nullptr &&
           index_buffer->Valid();
}

void GeometryVramTransfer::Prepare() {
    pimpl_->vertex_cache.Reset();
    pimpl_->index_cache.Reset();
}

void GeometryVramTransfer::Upload(std::vector<Geometry const *> const &geometries) {
    VkCommandBuffer cmds = pimpl_->BeginUpload();

    for (auto geometry : geometries) {
        pimpl_->Upload(geometry, cmds);
    }

    pimpl_->EndUpload(cmds);
}

GeometryVramTransfer::GpuGeometry GeometryVramTransfer::Find(Geometry const *geometry) {
    GpuGeometry result;

    result.vertex_buffer = pimpl_->vertex_cache.Find(geometry);
    result.index_buffer = pimpl_->index_cache.Find(geometry);
    assert(result.vertex_buffer != nullptr);
    assert(result.index_buffer != nullptr);

    result.index_element_type = geometry->indices.IndexElementType();

    return result;
}

} // namespace e8
