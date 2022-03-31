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
#include <filesystem>
#include <fstream>
#include <string>

#include "common/device.h"
#include "resource/buffer_index.h"
#include "resource/buffer_vertex.h"
#include "resource/geometry.h"
#include "resource/proto/geometry.pb.h"
#include "resource/proto/table.pb.h"

namespace e8 {
namespace {

std::filesystem::path GeometryFilePath(GeometryId const &id, ResourceTable const &table) {
    std::filesystem::path base_path(table.current_base_path());
    return base_path / (std::to_string(id) + ".gpb");
}

std::filesystem::path GeometryFileName(GeometryProto const &geometry) {
    return std::to_string(static_cast<GeometryId>(geometry.id())) + ".gpb";
}

std::filesystem::path GeometryFilePath(std::filesystem::path const &base_path,
                                       GeometryProto const &geometry) {
    return base_path / GeometryFileName(geometry);
}

} // namespace

Geometry::Geometry() : id(GenerateUuid()), rigidity(GeometryProto::INVALID) {}

Geometry::~Geometry() {}

void Geometry::FromDisk(GeometryId const &geometry_id, ResourceTable const &table,
                        VulkanContext *context) {
    // Loads protobuf object from disk.
    std::filesystem::path file_path = GeometryFilePath(geometry_id, table);
    std::fstream proto_file(file_path, std::ios::in | std::ios::binary);
    assert(proto_file.is_open());

    GeometryProto proto;
    assert(proto.ParseFromIstream(&proto_file));
    assert(static_cast<GeometryId>(proto.id()) == geometry_id);

    // Populates geometry metadata.
    id = geometry_id;
    rigidity = proto.rigidity();

    // Builds vertex and index buffers.
    vertices.CreateFromVertexListProto(proto.vertices(), context);
    indices.CreateFromIndexListProto(proto.primitives(), vertices.vertex_count, context);
}

void Geometry::ToDisk(bool temporary, ResourceTable *table) const {
    assert(vertices.Valid());
    assert(indices.Valid());

    GeometryProto proto;
    proto.set_id(id);
    proto.set_rigidity(rigidity);
    *proto.mutable_vertices() = vertices.ToProto();
    *proto.mutable_primitives() = indices.ToProto();

    SaveGeometryProto(proto, temporary, table);
}

bool Geometry::Valid() const { return vertices.Valid() && indices.Valid(); }

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
