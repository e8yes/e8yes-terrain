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
#include <cstdint>
#include <google/protobuf/repeated_field.h>
#include <limits>
#include <vulkan/vulkan.h>

#include "resource/buffer.h"
#include "resource/buffer_index.h"
#include "resource/proto/geometry.pb.h"

namespace e8 {
namespace {

unsigned IndexElementSize(unsigned vertex_count) {
    if (vertex_count <= std::numeric_limits<uint8_t>::max()) {
        // TODO: Uses UINT8 indices when supported.
        // return sizeof(uint8_t);
        return sizeof(uint16_t);
    } else if (vertex_count < std::numeric_limits<uint16_t>::max()) {
        return sizeof(uint16_t);
    } else if (vertex_count < std::numeric_limits<uint32_t>::max()) {
        return sizeof(uint32_t);
    } else {
        assert(false);
    }
}

unsigned ReadIndexElement(unsigned element, unsigned element_size, void const *source_buffer) {
    switch (element_size) {
    case sizeof(uint8_t): {
        return static_cast<uint8_t const *>(source_buffer)[element];
    }
    case sizeof(uint16_t): {
        return static_cast<uint16_t const *>(source_buffer)[element];
    }
    case sizeof(uint32_t): {
        return static_cast<uint32_t const *>(source_buffer)[element];
    }
    default: {
        assert(false);
    }
    }
}

void WriteIndexElement(unsigned element, unsigned element_size, unsigned index_value,
                       void *target_buffer) {
    switch (element_size) {
    case sizeof(uint8_t): {
        static_cast<uint8_t *>(target_buffer)[element] = static_cast<uint8_t>(index_value);
        break;
    }
    case sizeof(uint16_t): {
        static_cast<uint16_t *>(target_buffer)[element] = static_cast<uint16_t>(index_value);
        break;
    }
    case sizeof(uint32_t): {
        static_cast<uint32_t *>(target_buffer)[element] = static_cast<uint32_t>(index_value);
        break;
    }
    default: {
        assert(false);
    }
    }
}

} // namespace

StagingIndexBuffer::StagingIndexBuffer() {}

StagingIndexBuffer::~StagingIndexBuffer() {}

void StagingIndexBuffer::CreateFromIndexListProto(
    google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> const &primitive_list,
    unsigned vertex_count, VulkanContext *context) {
    index_count = primitive_list.size() * 3;
    index_element_size = IndexElementSize(vertex_count);

    this->Allocate(this->BufferSize(), context);

    void *target_buffer = this->BeginAccess();
    for (int i = 0; i < primitive_list.size(); ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            unsigned index_value = primitive_list[i].indices(j);
            WriteIndexElement(i * 3 + j, index_element_size, index_value, target_buffer);
        }
    }
    this->EndAccess();
}

google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> StagingIndexBuffer::ToProto() const {
    google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> index_list;

    if (!this->Valid()) {
        return index_list;
    }

    void const *source_buffer = this->BeginAccess();
    for (unsigned i = 0; i < index_count / 3; ++i) {
        PrimitiveIndicesProto *primitive = index_list.Add();

        for (unsigned j = 0; j < 3; ++j) {
            unsigned index_value = ReadIndexElement(i * 3 + j, index_element_size, source_buffer);
            primitive->add_indices(index_value);
        }
    }
    this->EndAccess();

    return index_list;
}

VkIndexType StagingIndexBuffer::IndexElementType() const {
    switch (index_element_size) {
    case sizeof(uint8_t): {
        return VkIndexType::VK_INDEX_TYPE_UINT8_EXT;
    }
    case sizeof(uint16_t): {
        return VkIndexType::VK_INDEX_TYPE_UINT16;
    }
    case sizeof(uint32_t): {
        return VkIndexType::VK_INDEX_TYPE_UINT32;
    }
    default: {
        assert(false);
    }
    }
}

unsigned StagingIndexBuffer::BufferSize() const { return index_count * index_element_size; }

} // namespace e8
