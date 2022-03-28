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

#ifndef ISLANDS_RESOURCE_BUFFER_INDEX_H
#define ISLANDS_RESOURCE_BUFFER_INDEX_H

#include <google/protobuf/repeated_field.h>
#include <vulkan/vulkan.h>

#include "resource/buffer.h"
#include "resource/proto/geometry.pb.h"

namespace e8 {

/**
 * @brief The StagingIndexBuffer class Wraps on a host-only Vulkan index buffer with a simply data
 * conversion layer. The index buffer is typically used for storing primitive indices of a geometry.
 */
class StagingIndexBuffer : public StagingBuffer {
  public:
    StagingIndexBuffer();
    StagingIndexBuffer(StagingIndexBuffer const &) = delete;
    StagingIndexBuffer(StagingIndexBuffer &&) = default;
    ~StagingIndexBuffer();

    StagingIndexBuffer &operator=(StagingIndexBuffer &&) = default;

    /**
     * @brief CreateFromIndexListProto Allocates memory for holding the index list and copies
     * index data into it. The size of each index element is minimized to just capable of
     * referencing the vertex list. The caller must specified the correct vertex count for it to
     * properly determine the index element size.
     */
    void CreateFromIndexListProto(
        google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> const &primitive_list,
        unsigned vertex_count, VulkanContext *context);

    /**
     * @brief ToProto Export the vertex buffer to a protobuf vertex list. If the vertex buffer is
     * invalid, it will return an empty list.
     */
    google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> ToProto() const;

    /**
     * @brief IndexElementType
     */
    VkIndexType IndexElementType() const;

    /**
     * @brief BufferSize
     */
    unsigned BufferSize() const;

    // The number of indices there is in this buffer.
    unsigned index_count;

    // The size of each index element, in byte.
    unsigned index_element_size;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_BUFFER_INDEX_H
