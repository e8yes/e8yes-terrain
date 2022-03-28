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

#include <google/protobuf/repeated_field.h>

#include "common/device.h"
#include "common/tensor.h"
#include "resource/buffer.h"
#include "resource/buffer_vertex.h"
#include "resource/common.h"
#include "resource/proto/geometry.pb.h"

namespace e8 {

PrimitiveVertex::PrimitiveVertex() {}

PrimitiveVertex::PrimitiveVertex(PrimitiveVertexProto const &proto)
    : position(ToVec3(proto.position())), normal(ToVec3(proto.normal())),
      tex_coord(ToVec2(proto.texcoord())) {}

PrimitiveVertex::~PrimitiveVertex() {}

PrimitiveVertexProto PrimitiveVertex::ToProto() const {
    PrimitiveVertexProto proto;
    *proto.mutable_position() = e8::ToProto(position);
    *proto.mutable_normal() = e8::ToProto(normal);
    *proto.mutable_texcoord() = e8::ToProto(tex_coord);
    return proto;
}

StagingVertexBuffer::StagingVertexBuffer() {}

StagingVertexBuffer::~StagingVertexBuffer() {}

void StagingVertexBuffer::CreateFromVertexListProto(
    google::protobuf::RepeatedPtrField<PrimitiveVertexProto> const &vertex_list,
    VulkanContext *context) {
    vertex_count = vertex_list.size();

    this->Allocate(this->BufferSize(), context);

    void *target_buffer = this->BeginAccess();
    for (unsigned i = 0; i < vertex_count; ++i) {
        static_cast<PrimitiveVertex *>(target_buffer)[i] = PrimitiveVertex(vertex_list[i]);
    }
    this->EndAccess();
}

google::protobuf::RepeatedPtrField<PrimitiveVertexProto> StagingVertexBuffer::ToProto() const {
    google::protobuf::RepeatedPtrField<PrimitiveVertexProto> vertex_list;

    if (!this->Valid()) {
        return vertex_list;
    }

    void const *source_buffer = this->BeginAccess();
    for (unsigned i = 0; i < vertex_count; ++i) {
        *vertex_list.Add() = static_cast<PrimitiveVertex const *>(source_buffer)[i].ToProto();
    }
    this->EndAccess();

    return vertex_list;
}

unsigned StagingVertexBuffer::BufferSize() const { return vertex_count * sizeof(PrimitiveVertex); }

} // namespace e8
