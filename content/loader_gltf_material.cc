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

#include "common/tensor.h"
#include "content/common.h"
#include "content/loader_gltf_material.h"
#include "resource/common.h"
#include "resource/proto/material.pb.h"
#include "resource/proto/texture.pb.h"
#include "third_party/tiny_gltf/tiny_gltf.h"

namespace e8 {
namespace {

TextureProto ToTextureProto(unsigned texture_index, tinygltf::Model const &model) {
    tinygltf::Texture const &texture = model.textures[texture_index];
    assert(texture.source >= 0);
    tinygltf::Image const &image = model.images[texture.source];

    TextureProto texture_proto;

    texture_proto.set_encoding(TextureProto::PNG);
    texture_proto.set_width(image.width);
    texture_proto.set_height(image.height);
    texture_proto.set_channel_count(image.component);
    texture_proto.set_channel_size(image.bits / 8);

    EncodeTextureData(image.image.data(), &texture_proto);

    return texture_proto;
}

void SplitMetallicRoughness(tinygltf::Image const &combined_image,
                            std::vector<uint8_t> *metallic_bitmap,
                            std::vector<uint8_t> *roughness_bitmap) {
    metallic_bitmap->resize(combined_image.width * combined_image.height);
    roughness_bitmap->resize(combined_image.width * combined_image.height);

    for (int i = 0; i < combined_image.height; ++i) {
        for (int j = 0; j < combined_image.width; ++j) {
            uint8_t roughness_value =
                combined_image.image[(j + i * combined_image.width) * combined_image.component + 1];
            uint8_t metallic_value =
                combined_image.image[(j + i * combined_image.width) * combined_image.component + 2];

            (*metallic_bitmap)[j + i * combined_image.width] = metallic_value;
            (*roughness_bitmap)[j + i * combined_image.width] = roughness_value;
        }
    }
}

void MetallicRoughnessToTextureProto(unsigned texture_index, tinygltf::Model const &model,
                                     TextureProto *metallic, TextureProto *roughness) {
    tinygltf::Texture const &texture = model.textures[texture_index];
    assert(texture.source >= 0);
    tinygltf::Image const &combined_image = model.images[texture.source];
    assert(combined_image.bits == 8);
    assert(combined_image.component == 3);

    std::vector<uint8_t> metallic_bitmap;
    std::vector<uint8_t> roughness_bitmap;
    SplitMetallicRoughness(combined_image, &metallic_bitmap, &roughness_bitmap);

    metallic->set_encoding(TextureProto::PNG);
    metallic->set_width(combined_image.width);
    metallic->set_height(combined_image.height);
    metallic->set_channel_count(1);
    metallic->set_channel_size(1);
    EncodeTextureData(metallic_bitmap.data(), metallic);

    roughness->set_encoding(TextureProto::PNG);
    roughness->set_width(combined_image.width);
    roughness->set_height(combined_image.height);
    roughness->set_channel_count(1);
    roughness->set_channel_size(1);
    EncodeTextureData(roughness_bitmap.data(), roughness);
}

} // namespace

MaterialProto LoadMaterial(tinygltf::Material const &material, tinygltf::Model const &model) {
    MaterialProto material_proto;

    material_proto.set_id(GenerateUuid());
    material_proto.set_name(material.name);

    if (material.pbrMetallicRoughness.baseColorTexture.index < 0) {
        *material_proto.mutable_albedo() = OneByOneAlbedoTexture(
            vec3{static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[0]),
                 static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[1]),
                 static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[2])});
    } else {
        *material_proto.mutable_albedo() =
            ToTextureProto(material.pbrMetallicRoughness.baseColorTexture.index, model);
        assert(material_proto.albedo().channel_size() == 1);
    }

    if (material.pbrMetallicRoughness.metallicRoughnessTexture.index < 0) {
        *material_proto.mutable_metallic() =
            OneByOneMetallicTexture(material.pbrMetallicRoughness.metallicFactor);
        *material_proto.mutable_roughness() =
            OneByOneRoughnessTexture(material.pbrMetallicRoughness.roughnessFactor);
    } else {
        MetallicRoughnessToTextureProto(
            material.pbrMetallicRoughness.metallicRoughnessTexture.index, model,
            material_proto.mutable_metallic(), material_proto.mutable_roughness());
    }

    if (material.normalTexture.index < 0) {
        *material_proto.mutable_normal() = OneByOneNormalTexture();
    } else {
        *material_proto.mutable_normal() = ToTextureProto(material.normalTexture.index, model);
    }

    return material_proto;
}

} // namespace e8
