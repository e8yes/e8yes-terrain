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

#include <boost/log/trivial.hpp>
#include <cstdint>
#include <google/protobuf/repeated_field.h>
#include <optional>
#include <string>
#include <vector>

#include "common/tensor.h"
#include "content/common.h"
#include "content/loader_gltf_geometry.h"
#include "resource/proto/geometry.pb.h"
#include "third_party/tiny_gltf/tiny_gltf.h"

namespace e8 {
namespace {

unsigned IndexElement(unsigned char const *index_buffer_data, unsigned stride, unsigned i) {
    switch (stride) {
    case 1:
        return reinterpret_cast<uint8_t const *>(index_buffer_data)[i];
    case 2:
        return reinterpret_cast<uint16_t const *>(index_buffer_data)[i];
    case 4:
        return reinterpret_cast<uint32_t const *>(index_buffer_data)[i];
    default:
        BOOST_LOG_TRIVIAL(warning) << "IndexElement(): Unknown stride=[" << stride << "].";
        return 0;
    }
}

google::protobuf::RepeatedField<float> Vec3Element(unsigned char const *attribute_buffer_data,
                                                   unsigned stride, unsigned i) {
    google::protobuf::RepeatedField<float> v;
    v.Resize(3, /*value=*/0);

    switch (stride) {
    case 4 * 3:
        for (unsigned j = 0; j < 3; ++j) {
            v.Set(j, reinterpret_cast<float const *>(attribute_buffer_data)[3 * i + j]);
        }
        break;
    case 8 * 3:
        for (unsigned j = 0; j < 3; ++j) {
            v.Set(j, static_cast<float>(
                         reinterpret_cast<double const *>(attribute_buffer_data)[3 * i + j]));
        }
        break;
    default:
        BOOST_LOG_TRIVIAL(warning) << "Vec3Element(): Unknown stride=[" << stride << "].";
        break;
    }

    return v;
}

google::protobuf::RepeatedField<float> Vec2Element(unsigned char const *attribute_buffer_data,
                                                   unsigned stride, unsigned i) {
    google::protobuf::RepeatedField<float> v;
    v.Resize(2, /*value=*/0);

    switch (stride) {
    case 4 * 2:
        for (unsigned j = 0; j < 2; ++j) {
            v.Set(j, reinterpret_cast<float const *>(attribute_buffer_data)[2 * i + j]);
        }
        break;
    case 8 * 2:
        for (unsigned j = 0; j < 2; ++j) {
            v.Set(j, static_cast<float>(
                         reinterpret_cast<double const *>(attribute_buffer_data)[2 * i + j]));
        }
        break;
    default:
        BOOST_LOG_TRIVIAL(warning) << "Vec2Element(): Unknown stride=[" << stride << "].";
    }

    return v;
}

bool HasRequiredVertexAttributes(tinygltf::Primitive const &primitive) {
    std::vector<std::string> required_attributes{"POSITION", "NORMAL", "TANGENT", "TEXCOORD_0"};

    for (auto const &required_attribute : required_attributes) {
        auto it = primitive.attributes.find(required_attribute);
        if (it == primitive.attributes.end()) {
            return false;
        }
    }

    return true;
}

std::vector<unsigned char> GetVertexBuffer(unsigned attribute_index, tinygltf::Model const &model,
                                           unsigned *stride, unsigned *count) {
    tinygltf::Accessor attribute_accessor = model.accessors[attribute_index];
    tinygltf::BufferView attribute_buffer_view = model.bufferViews[attribute_accessor.bufferView];
    tinygltf::Buffer attribute_buffer = model.buffers[attribute_buffer_view.buffer];

    *stride = attribute_accessor.ByteStride(attribute_buffer_view);
    *count = attribute_accessor.count;

    unsigned src_offset = attribute_accessor.byteOffset + attribute_buffer_view.byteOffset;
    unsigned num_bytes = *count * *stride;

    std::vector<unsigned char> buffer_data(num_bytes);
    for (unsigned i = 0; i < num_bytes; ++i) {
        buffer_data[i] = attribute_buffer.data[src_offset + i];
    }

    return buffer_data;
}

} // namespace

std::optional<GeometryProto> LoadGeometry(tinygltf::Primitive const &primitive,
                                          std::string const &name, tinygltf::Model const &model) {
    if (!HasRequiredVertexAttributes(primitive)) {
        BOOST_LOG_TRIVIAL(warning)
            << "LoadGeometry(): primitive=[" << name << "] doesn't have all required attributes.";
        return std::nullopt;
    }

    // Builds a geometry LOD proto with only the highest level of detail.
    GeometryProto geometry_proto;
    geometry_proto.set_id(GenerateUuid());
    geometry_proto.set_rigidity(GeometryProto::RIGID);

    // Loads vertex indices.
    tinygltf::Accessor index_accessor = model.accessors[primitive.indices];
    tinygltf::BufferView index_buffer_view = model.bufferViews[index_accessor.bufferView];
    tinygltf::Buffer index_buffer = model.buffers[index_buffer_view.buffer];

    unsigned char const *index_buffer_data =
        index_buffer.data.data() + index_accessor.byteOffset + index_buffer_view.byteOffset;
    unsigned stride = index_accessor.ByteStride(index_buffer_view);
    unsigned count = index_accessor.count;

    if (count % 3 != 0) {
        BOOST_LOG_TRIVIAL(warning)
            << "LoadGeometry(): primitive=[" << name << "] isn't a triangle mesh.";
        return std::nullopt;
    }

    for (unsigned i = 0; i < count / 3; ++i) {
        PrimitiveIndicesProto *primitive = geometry_proto.add_primitives();
        for (unsigned j = 0; j < 3; ++j) {
            unsigned index = IndexElement(index_buffer_data, stride, i * 3 + j);
            primitive->add_indices(index);
        }
    }

    // Loads vertex positions.
    unsigned position_stride;
    unsigned position_count;
    std::vector<unsigned char> position_data = GetVertexBuffer(
        primitive.attributes.at("POSITION"), model, &position_stride, &position_count);
    if (count == 0) {
        BOOST_LOG_TRIVIAL(warning) << "LoadGeometry(): primitive=[" << name
                                   << "attribute=[POSITION] is specified but empty.";
        return std::nullopt;
    }

    for (unsigned i = 0; i < position_count; i++) {
        *geometry_proto.add_vertices()->mutable_position() =
            Vec3Element(position_data.data(), position_stride, i);
    }

    // Loads vertex normal vectors.
    unsigned normal_stride;
    unsigned normal_count;
    std::vector<unsigned char> normal_data =
        GetVertexBuffer(primitive.attributes.at("NORMAL"), model, &normal_stride, &normal_count);
    if (normal_count != position_count) {
        BOOST_LOG_TRIVIAL(warning)
            << "LoadGeometry(): primitive=[" << name << "attribute=[NORMAL] has a different count=["
            << normal_count << "] than POSITION=[" << position_count << "].";
        return std::nullopt;
    }

    for (unsigned i = 0; i < normal_count; i++) {
        *geometry_proto.mutable_vertices(i)->mutable_normal() =
            Vec3Element(normal_data.data(), normal_stride, i);
    }

    // Loads vertex tangent vectors.
    unsigned tangent_stride;
    unsigned tangent_count;
    std::vector<unsigned char> tangent_data =
        GetVertexBuffer(primitive.attributes.at("TANGENT"), model, &tangent_stride, &tangent_count);
    if (tangent_count != position_count) {
        BOOST_LOG_TRIVIAL(warning) << "LoadGeometry(): primitive=[" << name
                                   << "attribute=[TANGENT] has a different count=[" << tangent_count
                                   << "] than POSITION=[" << position_count << "].";
        return std::nullopt;
    }

    for (unsigned i = 0; i < tangent_count; i++) {
        *geometry_proto.mutable_vertices(i)->mutable_tangent() =
            Vec3Element(tangent_data.data(), tangent_stride, i);
    }

    // Loads vertex texture coordinates.
    unsigned texcoord_stride;
    unsigned texcoord_count;
    std::vector<unsigned char> texcoord_data = GetVertexBuffer(
        primitive.attributes.at("TEXCOORD_0"), model, &texcoord_stride, &texcoord_count);
    if (texcoord_count != position_count) {
        BOOST_LOG_TRIVIAL(warning)
            << "LoadGeometry(): primitive=[" << name
            << " attribute=[TEXCOORD_0] has a different count=[" << texcoord_count
            << "] than POSITION=[" << position_count << "].";
        return std::nullopt;
    }

    for (unsigned i = 0; i < texcoord_count; i++) {
        *geometry_proto.mutable_vertices(i)->mutable_texcoord() =
            Vec2Element(texcoord_data.data(), texcoord_stride, i);
    }

    return geometry_proto;
}

} // namespace e8
