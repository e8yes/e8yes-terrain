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

#include <QVulkanWindow>
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "common/tensor.h"
#include "content/drawable.h"
#include "renderer/context.h"
#include "renderer/projection.h"
#include "renderer/scene.h"
#include "renderer/vram.h"

namespace e8 {
namespace {

unsigned VertexBufferSize(std::vector<PrimitiveVertex> const &vertices) {
    return vertices.size() * sizeof(vec3);
}

unsigned OptimalIndexSize(size_t num_vertices) {
    if (num_vertices <= 1 << 8) {
        return sizeof(uint8_t);
    } else if (num_vertices <= 1 << 16) {
        return sizeof(uint16_t);
    } else if (num_vertices <= 1UL << 32) {
        return sizeof(uint32_t);
    } else {
        assert(false);
    }
}

unsigned IndexBufferSize(std::vector<PrimitiveIndices> const &indices, unsigned index_size) {
    return indices.size() * 3 * index_size;
}

template <typename TargetMemoryIndexType>
void CopyIndices(std::vector<PrimitiveIndices> const &indices, void *target_memory) {
    for (unsigned i = 0; i < indices.size(); ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            *(reinterpret_cast<TargetMemoryIndexType *>(target_memory) + 3 * i + j) = indices[i](j);
        }
    }
}

} // namespace

class GeometryVramTransfer::GeometryVramTransferImpl {
  public:
    GeometryVramTransferImpl(unsigned capacity, VulkanContext *context);

    std::unordered_map<Drawable const *, UploadResult>::iterator Fetch(Drawable const *drawable);

    bool UploadVertices(std::vector<PrimitiveVertex> const &vertices,
                        std::optional<BufferUploadResult> *vertex_upload_result);

    bool UploadIndices(std::vector<PrimitiveIndices> const &indices, size_t num_vertices,
                       std::optional<BufferUploadResult> *index_upload_result,
                       VkIndexType *index_element_type);

  private:
    bool AllocateBuffer(unsigned new_size, VkBufferUsageFlags usage,
                        std::optional<BufferUploadResult> *buffer_upload_result);

    std::unordered_map<Drawable const *, UploadResult> cache_;
    VulkanContext *context_;
    unsigned used_;
    unsigned const capacity_;
};

GeometryVramTransfer::GeometryVramTransferImpl::GeometryVramTransferImpl(unsigned capacity,
                                                                         VulkanContext *context)
    : context_(context), used_(0), capacity_(capacity) {}

std::unordered_map<Drawable const *, GeometryVramTransfer::UploadResult>::iterator
GeometryVramTransfer::GeometryVramTransferImpl::Fetch(Drawable const *drawable) {
    auto it = cache_.find(drawable);
    if (it == cache_.end()) {
        it = cache_.insert(std::make_pair(drawable, UploadResult())).first;
    }
    return it;
}

bool GeometryVramTransfer::GeometryVramTransferImpl::AllocateBuffer(
    unsigned new_size, VkBufferUsageFlags usage,
    std::optional<BufferUploadResult> *buffer_upload_result) {
    if (buffer_upload_result->has_value() && new_size == buffer_upload_result->value().size) {
        // The size of the requested memory region remains the same. Returns the old memory region.
        assert(buffer_upload_result->value().buffer != nullptr);
        assert(buffer_upload_result->value().allocation != nullptr);
        return true;
    }

    if (buffer_upload_result->has_value()) {
        // An old memory region has been allocated. We need to re-allocate a region since the vertex
        // array is of different size now.
        assert(buffer_upload_result->value().buffer != nullptr);
        assert(buffer_upload_result->value().allocation != nullptr);

        vmaDestroyBuffer(context_->allocator, buffer_upload_result->value().buffer,
                         buffer_upload_result->value().allocation);

        used_ -= buffer_upload_result->value().size;

        buffer_upload_result->reset();
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
    *buffer_upload_result = BufferUploadResult();

    VkBufferCreateInfo vertex_buffer_info = {};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = new_size;
    vertex_buffer_info.usage = usage;

    VmaAllocationCreateInfo allocation_info = {};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    VkResult allocation_result = vmaCreateBuffer(
        context_->allocator, &vertex_buffer_info, &allocation_info,
        &buffer_upload_result->value().buffer, &buffer_upload_result->value().allocation, nullptr);
    if (allocation_result != VK_SUCCESS) {
        return false;
    }

    buffer_upload_result->value().size = new_size;
    used_ += new_size;
    return true;
}

bool GeometryVramTransfer::GeometryVramTransferImpl::UploadVertices(
    std::vector<PrimitiveVertex> const &vertices,
    std::optional<BufferUploadResult> *vertex_upload_result) {
    unsigned new_size = VertexBufferSize(vertices);
    if (!this->AllocateBuffer(new_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertex_upload_result)) {
        return false;
    }

    // Copies vertex data from CPU.
    void *vertex_buffer_region;
    VkResult result = vmaMapMemory(context_->allocator, vertex_upload_result->value().allocation,
                                   &vertex_buffer_region);
    if (result != VK_SUCCESS) {
        return false;
    }

    memcpy(vertex_buffer_region, vertices.data(), new_size);

    vmaUnmapMemory(context_->allocator, vertex_upload_result->value().allocation);

    return true;
}

bool GeometryVramTransfer::GeometryVramTransferImpl::UploadIndices(
    std::vector<PrimitiveIndices> const &indices, size_t num_vertices,
    std::optional<BufferUploadResult> *index_upload_result, VkIndexType *index_element_type) {
    unsigned index_size = OptimalIndexSize(num_vertices);
    unsigned new_size = IndexBufferSize(indices, index_size);
    if (!this->AllocateBuffer(new_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, index_upload_result)) {
        return false;
    }

    // Copies index data from CPU.
    void *index_buffer_region;
    VkResult result = vmaMapMemory(context_->allocator, index_upload_result->value().allocation,
                                   &index_buffer_region);
    if (result != VK_SUCCESS) {
        return false;
    }

    if (index_size == sizeof(uint8_t)) {
        CopyIndices<uint8_t>(indices, index_buffer_region);
        *index_element_type = VkIndexType::VK_INDEX_TYPE_UINT8_EXT;
    } else if (index_size == sizeof(uint16_t)) {
        CopyIndices<uint16_t>(indices, index_buffer_region);
        *index_element_type = VkIndexType::VK_INDEX_TYPE_UINT16;
    } else if (index_size == sizeof(uint32_t)) {
        CopyIndices<uint32_t>(indices, index_buffer_region);
        *index_element_type = VkIndexType::VK_INDEX_TYPE_UINT32;
    } else {
        assert(false);
    }

    vmaUnmapMemory(context_->allocator, index_upload_result->value().allocation);

    return true;
}

GeometryVramTransfer::BufferUploadResult::BufferUploadResult()
    : buffer(nullptr), allocation(nullptr), size(0) {}

GeometryVramTransfer::BufferUploadResult::~BufferUploadResult() {}

GeometryVramTransfer::GeometryVramTransfer(VulkanContext *context, unsigned capacity)
    : pimpl_(std::make_unique<GeometryVramTransfer::GeometryVramTransferImpl>(capacity, context)) {}

GeometryVramTransfer::~GeometryVramTransfer() {}

GeometryVramTransfer::UploadResult::UploadResult()
    : index_element_type(VkIndexType::VK_INDEX_TYPE_MAX_ENUM) {}

GeometryVramTransfer::UploadResult::~UploadResult() {}

GeometryVramTransfer::UploadResult GeometryVramTransfer::Upload(Drawable const *drawable) {
    auto &[_, cached_upload] = *pimpl_->Fetch(drawable);
    if (!cached_upload.vertex_buffer.has_value() || !cached_upload.index_buffer.has_value()) {
        // Data has never been uploaded before.
        pimpl_->UploadVertices(drawable->vertices, &cached_upload.vertex_buffer);
        pimpl_->UploadIndices(drawable->indices, drawable->vertices.size(),
                              &cached_upload.index_buffer, &cached_upload.index_element_type);
        return cached_upload;
    }

    switch (drawable->rigidity) {
    case Drawable::DEFORMABLE: {
        pimpl_->UploadVertices(drawable->vertices, &cached_upload.vertex_buffer);
        break;
    }
    case Drawable::TEARABLE: {
        pimpl_->UploadVertices(drawable->vertices, &cached_upload.vertex_buffer);
        pimpl_->UploadIndices(drawable->indices, drawable->vertices.size(),
                              &cached_upload.index_buffer, &cached_upload.index_element_type);
        break;
    }
    case Drawable::STATIC:
    case Drawable::RIGID: {
        // Nothing needs to be updated, supposedly.
        break;
    }
    }

    return cached_upload;
}

} // namespace e8
