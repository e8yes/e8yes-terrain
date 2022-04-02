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

#ifndef ISLANDS_RESOURCE_BUFFER_VERTEX_H
#define ISLANDS_RESOURCE_BUFFER_VERTEX_H

#include <google/protobuf/repeated_field.h>

#include "common/device.h"
#include "common/tensor.h"
#include "resource/buffer.h"
#include "resource/proto/geometry.pb.h"

namespace e8 {

/**
 * @brief The PrimitiveVertex struct It defines the geometric attributes of a primitive's
 * vertex.
 */
struct PrimitiveVertex {
    /**
     * @brief PrimitiveVertex Constructs a vertex with all attributes set to zero.
     */
    PrimitiveVertex();

    /**
     * @brief PrimitiveVertex Constructs a vertex from proto message.
     */
    PrimitiveVertex(PrimitiveVertexProto const &proto);

    ~PrimitiveVertex();

    /**
     * @brief ToProto Exports vertex attribute data as protobuf object.
     */
    PrimitiveVertexProto ToProto() const;

    // Defines the vertex's 3D position.
    vec3 position;

    // Defines the 3D normal vector for the vertex position.
    vec3 normal;

    // Defines the 3D tangent vector for the vertex position.
    vec3 tangent;

    // Defines the sign of the bitangent vector for the vertex position.
    float bitangent_sign;

    // Defines the normalized 2D coordinates of a texture image to make it wrap around the surface
    // surrounding this vertex.
    vec2 tex_coord;
};

/**
 * @brief The StagingVertexBuffer class Wraps on a host-only Vulkan vertex buffer with a simply data
 * conversion layer. The vertex buffer is typically used for storing vertices of a geometry.
 */
class StagingVertexBuffer : public StagingBuffer {
  public:
    StagingVertexBuffer();
    StagingVertexBuffer(StagingVertexBuffer const &) = delete;
    StagingVertexBuffer(StagingVertexBuffer &&) = default;
    ~StagingVertexBuffer();

    StagingVertexBuffer &operator=(StagingVertexBuffer &&) = default;

    /**
     * @brief CreateFromVertexListProto Allocates memory for holding the vertex list and copies
     * vertex data into it.
     */
    void CreateFromVertexListProto(
        google::protobuf::RepeatedPtrField<PrimitiveVertexProto> const &vertex_list,
        VulkanContext *context);

    /**
     * @brief ToProto Export the vertex buffer to a protobuf vertex list. If the vertex buffer is
     * invalid, it will return an empty list.
     */
    google::protobuf::RepeatedPtrField<PrimitiveVertexProto> ToProto() const;

    /**
     * @brief BufferSize The size of this vertex buffer, in bytes.
     */
    unsigned BufferSize() const;

    // The number of vertices there is in this buffer.
    unsigned vertex_count;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_BUFFER_VERTEX_H
