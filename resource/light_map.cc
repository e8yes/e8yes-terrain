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
#include "resource/buffer_texture.h"
#include "resource/common.h"
#include "resource/light_map.h"
#include "resource/proto/light_map.pb.h"
#include "resource/proto/table.pb.h"

namespace e8 {
namespace {

std::filesystem::path LightMapFilePath(LightMapId const &id, ResourceTable const &table) {
    std::filesystem::path base_path(table.current_base_path());
    return base_path / (std::to_string(id) + ".lpb");
}

std::filesystem::path LightMapFileName(LightMapProto const &light_map) {
    return std::to_string(static_cast<LightMapId>(light_map.id())) + ".lpb";
}

std::filesystem::path LightMapFilePath(std::filesystem::path const &base_path,
                                       LightMapProto const &light_map) {
    return base_path / LightMapFileName(light_map);
}

unsigned TextureSize(TextureProto const &texture) {
    return texture.width() * texture.height() * texture.channel_count() * texture.channel_size();
}

unsigned LightMapSize(LightMapProto const &light_map) { return TextureSize(light_map.radiosity()); }

} // namespace

LightMap::LightMap() : id(GenerateUuid()) {}

LightMap::~LightMap() {}

void LightMap::FromDisk(LightMapId const &id, ResourceTable const &table, VulkanContext *context) {
    // Loads protobuf object from disk.
    std::filesystem::path file_path = LightMapFilePath(id, table);
    std::fstream proto_file(file_path, std::ios::in | std::ios::binary);
    assert(proto_file.is_open());

    LightMapProto proto;
    assert(proto.ParseFromIstream(&proto_file));

    // Creates radiosity map on Vulkan memory regions.
    radiosity.CreateFromTextureProto(proto.radiosity(), context);
}

void LightMap::ToDisk(bool temporary, ResourceTable *table) {
    LightMapProto proto;
    proto.set_id(id);
    *proto.mutable_radiosity() = radiosity.ToProto();

    SaveLightMapProto(proto, temporary, table);
}

void SaveLightMapProto(LightMapProto const &light_map_proto, bool temporary, ResourceTable *table) {
    assert(light_map_proto.id() != kNullUuid);

    // Writes out the light map.
    std::filesystem::path file_path = LightMapFilePath(table->current_base_path(), light_map_proto);
    std::fstream proto_file(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    assert(proto_file.is_open());
    assert(light_map_proto.SerializeToOstream(&proto_file));
    proto_file.close();

    // Writes out the metadata for this light map resource.
    ResourceTable::Metadata metadata;
    metadata.set_file_name(LightMapFileName(light_map_proto).string());
    metadata.set_resource_size(LightMapSize(light_map_proto));
    metadata.set_temporary(temporary);

    (*table->mutable_light_maps())[light_map_proto.id()] = metadata;
}

} // namespace e8
