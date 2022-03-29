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
#include "resource/common.h"
#include "resource/material.h"
#include "resource/proto/material.pb.h"
#include "resource/proto/texture.pb.h"

namespace e8 {
namespace {

std::filesystem::path MaterialFilePath(MaterialId const &id, ResourceTable const &table) {
    std::filesystem::path base_path(table.current_base_path());
    return base_path / (std::to_string(id) + ".mpb");
}

std::filesystem::path MaterialFileName(MaterialProto const &material) {
    return std::to_string(static_cast<MaterialId>(material.id())) + ".mpb";
}

std::filesystem::path MaterialFilePath(std::filesystem::path const &base_path,
                                       MaterialProto const &material) {
    return base_path / MaterialFileName(material);
}

unsigned TextureSize(TextureProto const &texture) {
    return texture.width() * texture.height() * texture.channel_count() * texture.channel_size();
}

unsigned MaterialSize(MaterialProto const &material) {
    return TextureSize(material.albedo()) + TextureSize(material.normal()) +
           TextureSize(material.roughness());
}

} // namespace

Material::Material() : id(GenerateUuid()) {}

Material::~Material() {}

void Material::FromDisk(MaterialId const &id, ResourceTable const &table, VulkanContext *context) {
    // Loads protobuf object from disk.
    std::filesystem::path file_path = MaterialFilePath(id, table);
    std::fstream proto_file(file_path, std::ios::in | std::ios::binary);
    assert(proto_file.is_open());

    MaterialProto proto;
    assert(proto.ParseFromIstream(&proto_file));

    // Creates texture maps on Vulkan memory regions.
    albedo.CreateFromTextureProto(proto.albedo(), context);
    normal.CreateFromTextureProto(proto.normal(), context);
    metallic.CreateFromTextureProto(proto.metallic(), context);
    roughness.CreateFromTextureProto(proto.roughness(), context);
}

void Material::ToDisk(bool temporary, ResourceTable *table) {
    MaterialProto proto;

    proto.set_id(id);
    proto.set_name(name);
    *proto.mutable_albedo() = albedo.ToProto();
    *proto.mutable_normal() = normal.ToProto();
    *proto.mutable_metallic() = metallic.ToProto();
    *proto.mutable_roughness() = roughness.ToProto();

    SaveMaterialProto(proto, temporary, table);
}

void SaveMaterialProto(MaterialProto const &material_proto, bool temporary, ResourceTable *table) {
    assert(material_proto.id() != kNullUuid);

    // Writes out the material.
    std::filesystem::path file_path = MaterialFilePath(table->current_base_path(), material_proto);
    std::fstream proto_file(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    assert(proto_file.is_open());
    assert(material_proto.SerializeToOstream(&proto_file));
    proto_file.close();

    // Writes out the metadata for this material resource.
    ResourceTable::Metadata metadata;
    metadata.set_file_name(MaterialFileName(material_proto));
    metadata.set_resource_size(MaterialSize(material_proto));
    metadata.set_temporary(temporary);

    (*table->mutable_materials())[material_proto.id()] = metadata;
}

} // namespace e8
