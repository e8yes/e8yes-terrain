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
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

#include "resource/common.h"
#include "resource/geometry.h"
#include "resource/proto/geometry.pb.h"

namespace e8 {
namespace {

std::filesystem::path GeometryFilePath(GeometryId const &id, ResourceTable const &table) {
    std::filesystem::path base_path(table.current_base_path());
    return base_path / (std::to_string(id) + ".gpb");
}

std::filesystem::path GeometryFileName(GeometryProto const &geometry) {
    return std::to_string(geometry.id()) + ".gpb";
}

std::filesystem::path GeometryFilePath(std::filesystem::path const &base_path,
                                       GeometryProto const &geometry) {
    return base_path / GeometryFileName(geometry);
}

template <typename IndexElementType>
void ToIndexProto(IndexElementType const *index_list, unsigned num_indices,
                  google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> *primitives) {
    for (unsigned i = 0; i < num_indices / 3; ++i) {
        PrimitiveIndicesProto *primitive = primitives->Add();
        for (unsigned j = 0; j < 3; ++j) {
            primitive->add_indices(index_list[j + i * 3]);
        }
    }
}

template <typename IndexElementType>
void FromIndexProto(google::protobuf::RepeatedPtrField<PrimitiveIndicesProto> const &primitives,
                    IndexElementType *index_list) {
    for (int i = 0; i < primitives.size(); ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            index_list[j + i * 3] = primitives.at(i).indices(j);
        }
    }
}

} // namespace

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

Geometry::VulkanBuffer::VulkanBuffer()
    : buffer(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), context(nullptr) {}

Geometry::VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) { *this = std::move(other); }

Geometry::VulkanBuffer::~VulkanBuffer() { this->Free(); }

Geometry::VulkanBuffer &Geometry::VulkanBuffer::operator=(VulkanBuffer &&other) {
    this->Free();

    buffer = other.buffer;
    allocation = other.allocation;
    context = other.context;

    other.buffer = nullptr;
    other.allocation = nullptr;

    return *this;
}

void Geometry::VulkanBuffer::Allocate(unsigned size, VulkanContext *context) {
    this->Free();

    VkBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = size;
    vertex_buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info, &buffer, &allocation,
                    /*pAllocationInfo=*/nullptr);

    this->context = context;
}

void Geometry::VulkanBuffer::Free() {
    if (!this->Valid()) {
        return;
    }

    vmaDestroyBuffer(context->allocator, buffer, allocation);
    buffer = VK_NULL_HANDLE;
    allocation = VK_NULL_HANDLE;
}

void *Geometry::VulkanBuffer::BeginAccess() const {
    assert(this->Valid());
    void *data;
    vmaMapMemory(context->allocator, allocation, &data);
    return data;
}

void Geometry::VulkanBuffer::EndAccess() const {
    assert(this->Valid());
    vmaUnmapMemory(context->allocator, allocation);
}

bool Geometry::VulkanBuffer::Valid() const { return buffer != VK_NULL_HANDLE; }

Geometry::Geometry()
    : id(GenerateUuid()), rigidity(GeometryProto::INVALID), vertex_count(0), index_count(0) {}

Geometry::~Geometry() {}

void Geometry::FromDisk(GeometryId const &geometry_id, ResourceTable const &table,
                        VulkanContext *context) {
    // Loads protobuf object from disk.
    std::filesystem::path file_path = GeometryFilePath(geometry_id, table);
    std::fstream proto_file(file_path, std::ios::in | std::ios::binary);
    assert(proto_file.is_open());

    GeometryProto proto;
    assert(proto.ParseFromIstream(&proto_file));

    // Builds vertex and index buffers.
    vertex_count = proto.vertices_size();
    index_count = proto.primitives_size() * 3;

    vertices.Allocate(this->VertexBufferSize(), context);
    indices.Allocate(this->IndexBufferSize(), context);

    // Populuates the vertex buffer.
    PrimitiveVertex *vertex_list = static_cast<PrimitiveVertex *>(vertices.BeginAccess());
    for (unsigned i = 0; i < vertex_count; ++i) {
        vertex_list[i] = PrimitiveVertex(proto.vertices(i));
    }
    vertices.EndAccess();

    // Populates the index buffer.
    void *index_list = indices.BeginAccess();
    switch (this->IndexElementType()) {
    case VK_INDEX_TYPE_UINT8_EXT:
        FromIndexProto<uint8_t>(proto.primitives(), static_cast<uint8_t *>(index_list));
        break;
    case VK_INDEX_TYPE_UINT16:
        FromIndexProto<uint16_t>(proto.primitives(), static_cast<uint16_t *>(index_list));
        break;
    case VK_INDEX_TYPE_UINT32:
        FromIndexProto<uint16_t>(proto.primitives(), static_cast<uint16_t *>(index_list));
        break;
    default:
        assert(false);
    }
    indices.EndAccess();
}

void Geometry::ToDisk(bool temporary, ResourceTable *table) const {
    assert(vertices.Valid());
    assert(indices.Valid());

    GeometryProto proto;
    proto.set_id(id);
    proto.set_rigidity(rigidity);

    // Extracts vertex data.
    PrimitiveVertex *vertex_list = static_cast<PrimitiveVertex *>(vertices.BeginAccess());
    for (unsigned i = 0; i < vertex_count; ++i) {
        *proto.add_vertices() = vertex_list[i].ToProto();
    }
    vertices.EndAccess();

    // Extracts index data.
    void *index_list = indices.BeginAccess();
    switch (this->IndexElementType()) {
    case VK_INDEX_TYPE_UINT8_EXT:
        e8::ToIndexProto<uint8_t>(static_cast<uint8_t *>(index_list), index_count,
                                  proto.mutable_primitives());
        break;
    case VK_INDEX_TYPE_UINT16:
        e8::ToIndexProto<uint16_t>(static_cast<uint16_t *>(index_list), index_count,
                                   proto.mutable_primitives());
        break;
    case VK_INDEX_TYPE_UINT32:
        e8::ToIndexProto<uint32_t>(static_cast<uint32_t *>(index_list), index_count,
                                   proto.mutable_primitives());
        break;
    default:
        assert(false);
    }
    indices.EndAccess();

    SaveGeometryProto(proto, temporary, table);
}

VkIndexType Geometry::IndexElementType() const {
    if (vertex_count <= std::numeric_limits<uint8_t>::max()) {
        // TODO: Uses UINT8 indices when supported.
        // return VkIndexType::VK_INDEX_TYPE_UINT8_EXT;
        return VkIndexType::VK_INDEX_TYPE_UINT16;
    } else if (vertex_count < std::numeric_limits<uint16_t>::max()) {
        return VkIndexType::VK_INDEX_TYPE_UINT16;
    } else if (vertex_count < std::numeric_limits<uint32_t>::max()) {
        return VkIndexType::VK_INDEX_TYPE_UINT32;
    } else {
        assert(false);
    }
}

unsigned Geometry::IndexElementSize() const {
    switch (this->IndexElementType()) {
    case VkIndexType::VK_INDEX_TYPE_UINT8_EXT: {
        return sizeof(uint8_t);
    }
    case VkIndexType::VK_INDEX_TYPE_UINT16: {
        return sizeof(uint16_t);
    }
    case VkIndexType::VK_INDEX_TYPE_UINT32: {
        return sizeof(uint32_t);
    }
    default: {
        assert(false);
    }
    }
}

uint64_t Geometry::VertexBufferSize() const { return vertex_count * sizeof(PrimitiveVertex); }

uint64_t Geometry::IndexBufferSize() const { return index_count * this->IndexElementSize(); }

void SaveGeometryProto(GeometryProto const &geometry_proto, bool temporary, ResourceTable *table) {
    assert(geometry_proto.id() != kNullUuid);

    // Writes out the geometry.
    std::filesystem::path file_path = GeometryFilePath(table->current_base_path(), geometry_proto);
    std::fstream proto_file(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    assert(proto_file.is_open());
    assert(geometry_proto.SerializeToOstream(&proto_file));
    proto_file.close();

    // Writes out the metadata for this geometry resource.
    ResourceTable::Metadata metadata;
    metadata.set_file_name(GeometryFileName(geometry_proto));
    metadata.set_resource_size(geometry_proto.ByteSizeLong());
    metadata.set_temporary(temporary);

    (*table->mutable_geometries())[geometry_proto.id()] = metadata;
}

} // namespace e8
