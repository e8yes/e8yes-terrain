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
#include <unordered_map>
#include <vector>

#include "common/tensor.h"
#include "content/common.h"
#include "content/geometry.h"
#include "renderer/context.h"
#include "renderer/projection.h"
#include "renderer/vram_geometry.h"

namespace e8 {
namespace {

unsigned VertexBufferSize(std::vector<PrimitiveVertex> const &vertices) {
    return vertices.size() * sizeof(PrimitiveVertex);
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

unsigned IndexBufferSize(std::vector<Primitive> const &primitives, unsigned index_size) {
    return primitives.size() * 3 * index_size;
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

} // namespace

class GeometryVramTransfer::GeometryVramTransferImpl {
  public:
    GeometryVramTransferImpl(unsigned capacity, VulkanContext *context);

    std::unordered_map<Geometry const *, UploadResult>::iterator Fetch(Geometry const *geometry);

    bool UploadVertices(std::vector<PrimitiveVertex> const &vertices,
                        std::unique_ptr<BufferUploadResult> *vertex_upload_result);

    bool UploadIndices(std::vector<Primitive> const &primitives, size_t num_vertices,
                       std::unique_ptr<BufferUploadResult> *index_upload_result,
                       VkIndexType *index_element_type);

  private:
    bool AllocateBuffer(unsigned new_size, VkBufferUsageFlags usage,
                        std::unique_ptr<BufferUploadResult> *buffer_upload_result);

    std::unordered_map<Geometry const *, UploadResult> cache_;
    VulkanContext *context_;
    unsigned used_;
    unsigned const capacity_;
};

GeometryVramTransfer::GeometryVramTransferImpl::GeometryVramTransferImpl(unsigned capacity,
                                                                         VulkanContext *context)
    : context_(context), used_(0), capacity_(capacity) {}

std::unordered_map<Geometry const *, GeometryVramTransfer::UploadResult>::iterator
GeometryVramTransfer::GeometryVramTransferImpl::Fetch(Geometry const *geometry) {
    auto it = cache_.find(geometry);
    if (it == cache_.end()) {
        it = cache_.insert(std::make_pair(geometry, UploadResult())).first;
    }
    return it;
}

bool GeometryVramTransfer::GeometryVramTransferImpl::AllocateBuffer(
    unsigned new_size, VkBufferUsageFlags usage,
    std::unique_ptr<BufferUploadResult> *buffer_upload_result) {
    if (*buffer_upload_result != nullptr && new_size == (*buffer_upload_result)->size) {
        // The size of the requested memory region remains the same. Returns the old memory region.
        assert((*buffer_upload_result)->buffer != nullptr);
        assert((*buffer_upload_result)->allocation != nullptr);
        return true;
    }

    if (*buffer_upload_result != nullptr) {
        // An old memory region has been allocated. We need to re-allocate a region since the vertex
        // array is of different size now.
        assert((*buffer_upload_result)->buffer != nullptr);
        assert((*buffer_upload_result)->allocation != nullptr);

        used_ -= (*buffer_upload_result)->size;
        *buffer_upload_result = nullptr;
    }

    if (new_size == 0) {
        // Can't allocate for an empty geometry.
        return false;
    }

    if (used_ + new_size > capacity_) {
        // TODO: Destroys LRU items to make room for new objects.
        return false;
    }

    // Allocates a new memory region of new_size.
    *buffer_upload_result = std::make_unique<BufferUploadResult>(usage, new_size, context_);

    used_ += new_size;
    return true;
}

bool GeometryVramTransfer::GeometryVramTransferImpl::UploadVertices(
    std::vector<PrimitiveVertex> const &vertices,
    std::unique_ptr<BufferUploadResult> *vertex_upload_result) {
    unsigned new_size = VertexBufferSize(vertices);
    if (!this->AllocateBuffer(new_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertex_upload_result)) {
        return false;
    }

    // Copies vertex data from CPU.
    void *vertex_buffer_region;
    assert(VK_SUCCESS == vmaMapMemory(context_->allocator, (*vertex_upload_result)->allocation,
                                      &vertex_buffer_region));

    memcpy(vertex_buffer_region, vertices.data(), new_size);

    vmaUnmapMemory(context_->allocator, (*vertex_upload_result)->allocation);

    return true;
}

bool GeometryVramTransfer::GeometryVramTransferImpl::UploadIndices(
    std::vector<Primitive> const &primitives, size_t num_vertices,
    std::unique_ptr<BufferUploadResult> *index_upload_result, VkIndexType *index_element_type) {
    unsigned index_size = OptimalIndexSize(num_vertices);
    unsigned new_size = IndexBufferSize(primitives, index_size);
    if (!this->AllocateBuffer(new_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, index_upload_result)) {
        return false;
    }

    // Copies index data from CPU.
    void *index_buffer_region;
    assert(VK_SUCCESS == vmaMapMemory(context_->allocator, (*index_upload_result)->allocation,
                                      &index_buffer_region));

    if (index_size == sizeof(uint8_t)) {
        CopyIndices<uint8_t>(primitives, index_buffer_region);
        *index_element_type = VkIndexType::VK_INDEX_TYPE_UINT8_EXT;
    } else if (index_size == sizeof(uint16_t)) {
        CopyIndices<uint16_t>(primitives, index_buffer_region);
        *index_element_type = VkIndexType::VK_INDEX_TYPE_UINT16;
    } else if (index_size == sizeof(uint32_t)) {
        CopyIndices<uint32_t>(primitives, index_buffer_region);
        *index_element_type = VkIndexType::VK_INDEX_TYPE_UINT32;
    } else {
        assert(false);
    }

    vmaUnmapMemory(context_->allocator, (*index_upload_result)->allocation);

    return true;
}

GeometryVramTransfer::BufferUploadResult::BufferUploadResult(VkBufferUsageFlags usage,
                                                             unsigned size, VulkanContext *context)
    : buffer(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), size(size), context(context) {
    VkBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = size;
    vertex_buffer_info.usage = usage;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info, &buffer, &allocation,
                    /*pAllocationInfo=*/nullptr);
}

GeometryVramTransfer::BufferUploadResult::~BufferUploadResult() {
    if (this->Valid()) {
        vmaDestroyBuffer(context->allocator, buffer, allocation);
    }
}

bool GeometryVramTransfer::BufferUploadResult::Valid() const {
    return buffer != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE;
}

GeometryVramTransfer::GeometryVramTransfer(VulkanContext *context, unsigned capacity)
    : pimpl_(std::make_unique<GeometryVramTransfer::GeometryVramTransferImpl>(capacity, context)) {}

GeometryVramTransfer::~GeometryVramTransfer() {}

GeometryVramTransfer::UploadResult::UploadResult()
    : index_element_type(VkIndexType::VK_INDEX_TYPE_MAX_ENUM) {}

GeometryVramTransfer::UploadResult::~UploadResult() {}

bool GeometryVramTransfer::UploadResult::Valid() const {
    return vertex_buffer != nullptr && vertex_buffer->Valid() && index_buffer != nullptr &&
           index_buffer->Valid();
}

GeometryVramTransfer::UploadResult const *GeometryVramTransfer::Upload(Geometry const *geometry) {
    auto &[_, cached_upload] = *pimpl_->Fetch(geometry);
    if (cached_upload.vertex_buffer == nullptr || cached_upload.index_buffer == nullptr) {
        // Data has never been uploaded before.
        pimpl_->UploadVertices(geometry->vertices, &cached_upload.vertex_buffer);
        pimpl_->UploadIndices(geometry->primitives, geometry->vertices.size(),
                              &cached_upload.index_buffer, &cached_upload.index_element_type);
        return &cached_upload;
    }

    switch (geometry->rigidity) {
    case GeometryProto::DEFORMABLE: {
        pimpl_->UploadVertices(geometry->vertices, &cached_upload.vertex_buffer);
        break;
    }
    case GeometryProto::TEARABLE: {
        pimpl_->UploadVertices(geometry->vertices, &cached_upload.vertex_buffer);
        pimpl_->UploadIndices(geometry->primitives, geometry->vertices.size(),
                              &cached_upload.index_buffer, &cached_upload.index_element_type);
        break;
    }
    case GeometryProto::STATIC:
    case GeometryProto::RIGID: {
        // Nothing needs to be updated, supposedly.
        break;
    }
    default: {
        assert(false);
        break;
    }
    }

    return &cached_upload;
}

} // namespace e8
