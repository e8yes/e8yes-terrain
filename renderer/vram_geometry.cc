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
#include <cstdint>
#include <memory>
#include <vector>

#include "content/geometry.h"
#include "content/proto/camera.pb.h"
#include "renderer/context.h"
#include "renderer/vram_cache.h"
#include "renderer/vram_geometry.h"

namespace e8 {
namespace {

uint64_t VertexBufferSize(Geometry const *geometry) {
    return geometry->vertices.size() * sizeof(PrimitiveVertex);
}

void UploadVertexBuffer(Geometry const *geometry, uint64_t old_buffer_size,
                        uint64_t new_buffer_size, VulkanContext *context,
                        GeometryVramTransfer::BufferUploadResult *vertex_buffer_object) {
    if (new_buffer_size != old_buffer_size) {
        vertex_buffer_object->Allocate(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, new_buffer_size, context);
    }

    // Copies vertex data to a staging buffer.
    void *vertex_buffer_region;
    assert(VK_SUCCESS == vmaMapMemory(context->allocator, vertex_buffer_object->allocation,
                                      &vertex_buffer_region));

    memcpy(vertex_buffer_region, geometry->vertices.data(), new_buffer_size);

    vmaUnmapMemory(context->allocator, vertex_buffer_object->allocation);
}

template <typename TargetMemoryIndexType>
void CopyIndices(std::vector<Primitive> const &primitives, void *target_memory) {
    for (unsigned i = 0; i < primitives.size(); ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            *(reinterpret_cast<TargetMemoryIndexType *>(target_memory) + 3 * i + j) =
                static_cast<TargetMemoryIndexType>(primitives[i].vertex_refs(j));
        }
    }
}

unsigned OptimalIndexSize(size_t num_vertices) {
    if (num_vertices <= 1 << 8) {
        // TODO: Adds indexTypeUint8 device feature.
        // return sizeof(uint8_t);
        return sizeof(uint16_t);
    } else if (num_vertices <= 1 << 16) {
        return sizeof(uint16_t);
    } else if (num_vertices <= 1UL << 32) {
        return sizeof(uint32_t);
    } else {
        assert(false);
    }
}

VkIndexType IndexType(size_t num_vertices) {
    unsigned index_size = OptimalIndexSize(num_vertices);

    if (index_size == sizeof(uint8_t)) {
        return VkIndexType::VK_INDEX_TYPE_UINT8_EXT;
    } else if (index_size == sizeof(uint16_t)) {
        return VkIndexType::VK_INDEX_TYPE_UINT16;
    } else if (index_size == sizeof(uint32_t)) {
        return VkIndexType::VK_INDEX_TYPE_UINT32;
    } else {
        assert(false);
    }
}

unsigned IndexBufferSize(Geometry const *geometry) {
    return geometry->primitives.size() * 3 * OptimalIndexSize(geometry->vertices.size());
}

void UploadIndexBuffer(Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
                       VulkanContext *context,
                       GeometryVramTransfer::BufferUploadResult *index_buffer_object) {
    if (new_buffer_size != old_buffer_size) {
        index_buffer_object->Allocate(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, new_buffer_size, context);
    }

    // Copies index data from to a staging buffer.
    void *index_buffer_region;
    assert(VK_SUCCESS ==
           vmaMapMemory(context->allocator, index_buffer_object->allocation, &index_buffer_region));

    switch (IndexType(geometry->vertices.size())) {
    case VkIndexType::VK_INDEX_TYPE_UINT8_EXT: {
        CopyIndices<uint8_t>(geometry->primitives, index_buffer_region);
        break;
    }
    case VkIndexType::VK_INDEX_TYPE_UINT16: {
        CopyIndices<uint16_t>(geometry->primitives, index_buffer_region);
        break;
    }
    case VkIndexType::VK_INDEX_TYPE_UINT32: {
        CopyIndices<uint32_t>(geometry->primitives, index_buffer_region);
        break;
    }
    default: {
        assert(false);
    }
    }

    vmaUnmapMemory(context->allocator, index_buffer_object->allocation);
}

} // namespace

struct GeometryVramTransfer::GeometryVramTransferImpl {
    GeometryVramTransferImpl(VulkanContext *context);
    ~GeometryVramTransferImpl();

    ConstrainedLruVramCache<Geometry const *, BufferUploadResult> vertex_cache;
    ConstrainedLruVramCache<Geometry const *, BufferUploadResult> index_cache;
    VulkanContext *context;
};

GeometryVramTransfer::GeometryVramTransferImpl::GeometryVramTransferImpl(VulkanContext *context)
    : vertex_cache(&context->geometry_usage), index_cache(&context->geometry_usage),
      context(context) {}

GeometryVramTransfer::GeometryVramTransferImpl::~GeometryVramTransferImpl() {}

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
    vertex_buffer_info.usage = usage;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info, &buffer, &allocation,
                    /*pAllocationInfo=*/nullptr);
}

void GeometryVramTransfer::BufferUploadResult::Free() {
    if (this->Valid()) {
        vmaDestroyBuffer(context->allocator, buffer, allocation);
    }
}

bool GeometryVramTransfer::BufferUploadResult::Valid() const {
    return buffer != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE;
}

GeometryVramTransfer::GeometryVramTransfer(VulkanContext *context)
    : pimpl_(std::make_unique<GeometryVramTransfer::GeometryVramTransferImpl>(context)) {}

GeometryVramTransfer::~GeometryVramTransfer() {}

GeometryVramTransfer::UploadResult::UploadResult()
    : vertex_buffer(nullptr), index_buffer(nullptr),
      index_element_type(VkIndexType::VK_INDEX_TYPE_MAX_ENUM) {}

GeometryVramTransfer::UploadResult::~UploadResult() {}

bool GeometryVramTransfer::UploadResult::Valid() const {
    return vertex_buffer != nullptr && vertex_buffer->Valid() && index_buffer != nullptr &&
           index_buffer->Valid();
}

GeometryVramTransfer::UploadResult GeometryVramTransfer::Upload(Geometry const *geometry) {
    UploadResult result;

    result.vertex_buffer = pimpl_->vertex_cache.Upload(
        geometry,
        /*always_upload=*/geometry->rigidity == GeometryProto::DEFORMABLE ||
            geometry->rigidity == GeometryProto::TEARABLE,
        VertexBufferSize, /*upload_fn=*/
        [this](Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
               GeometryVramTransfer::BufferUploadResult *vertex_buffer_object) {
            UploadVertexBuffer(geometry, old_buffer_size, new_buffer_size, this->pimpl_->context,
                               vertex_buffer_object);
        });

    result.index_buffer = pimpl_->index_cache.Upload(
        geometry, /*always_upload=*/geometry->rigidity == GeometryProto::TEARABLE, IndexBufferSize,
        /*upload_fn=*/
        [this](Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
               GeometryVramTransfer::BufferUploadResult *vertex_buffer_object) {
            UploadIndexBuffer(geometry, old_buffer_size, new_buffer_size, this->pimpl_->context,
                              vertex_buffer_object);
        });

    result.index_element_type = IndexType(geometry->vertices.size());

    return result;
}

} // namespace e8
