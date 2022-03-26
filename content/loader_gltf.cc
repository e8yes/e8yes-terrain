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
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/device.h"
#include "content/loader_gltf.h"
#include "content/proto/lod.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "resource/accessor.h"
#include "resource/common.h"
#include "resource/geometry.h"
#include "third_party/tiny_gltf/tiny_gltf.h"

namespace e8 {
namespace {

static std::string FileExtension(std::string const &gltf_file_path) {
    if (gltf_file_path.find_last_of(".") != std::string::npos) {
        return gltf_file_path.substr(gltf_file_path.find_last_of(".") + 1);
    }
    return "";
}

bool BinaryGltf(std::string const &gltf_file_path) {
    return "glb" == FileExtension(gltf_file_path);
}

aabb BoundingBoxOf(GeometryProto const &geometry) {
    aabb bounding_box;
    for (auto const &vertex : geometry.vertices()) {
        bounding_box = bounding_box + ToVec3(vertex.position());
    }
    return bounding_box;
}

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

mat44 NodeTransform(tinygltf::Node const &node) {
    if (node.matrix.empty()) {
        // SRT-like transform specification.
        mat44 transform = mat44_identity();

        if (!node.scale.empty()) {
            transform = transform * mat44_scale(vec3{static_cast<float>(node.scale[0]),
                                                     static_cast<float>(node.scale[1]),
                                                     static_cast<float>(node.scale[2])});
        }

        if (!node.rotation.empty()) {
            transform =
                transform *
                mat44_rotate(node.rotation[3], /*axis=*/vec3{static_cast<float>(node.rotation[0]),
                                                             static_cast<float>(node.rotation[1]),
                                                             static_cast<float>(node.rotation[2])});
        }

        if (!node.translation.empty()) {
            transform = transform * mat44_translate(vec3{static_cast<float>(node.translation[0]),
                                                         static_cast<float>(node.translation[1]),
                                                         static_cast<float>(node.translation[2])});
        }

        return transform;
    }

    mat44 transform;
    for (unsigned j = 0; j < 4; ++j) {
        for (unsigned i = 0; i < 4; ++i) {
            transform(i, j) = node.matrix[i + j * 4];
        }
    }

    return transform;
}

bool HasRequiredVertexAttributes(tinygltf::Primitive const &primitive) {
    std::vector<std::string> required_attributes{"POSITION", "NORMAL", "TEXCOORD_0"};

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

std::optional<GeometryProto> LoadGeometry(tinygltf::Primitive const &primitive,
                                          tinygltf::Model const &model) {
    if (!HasRequiredVertexAttributes(primitive)) {
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

    assert(count % 3 == 0);

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
        BOOST_LOG_TRIVIAL(warning)
            << "LoadGeometry(): attribute=[POSITION] is specified but empty.";
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
        BOOST_LOG_TRIVIAL(warning) << "LoadGeometry(): attribute=[NORMAL] has a different count=["
                                   << normal_count << "] than POSITION=[" << position_count << "].";
        return std::nullopt;
    }

    for (unsigned i = 0; i < normal_count; i++) {
        *geometry_proto.mutable_vertices(i)->mutable_normal() =
            Vec3Element(normal_data.data(), normal_stride, i);
    }

    // Loads vertex texture coordinates.
    unsigned texcoord_stride;
    unsigned texcoord_count;
    std::vector<unsigned char> texcoord_data = GetVertexBuffer(
        primitive.attributes.at("TEXCOORD_0"), model, &texcoord_stride, &texcoord_count);
    if (texcoord_count != position_count) {
        BOOST_LOG_TRIVIAL(warning)
            << "LoadGeometry(): attribute=[TEXCOORD_0] has a different count=[" << texcoord_count
            << "] than POSITION=[" << position_count << "].";
        return std::nullopt;
    }

    for (unsigned i = 0; i < texcoord_count; i++) {
        *geometry_proto.mutable_vertices(i)->mutable_texcoord() =
            Vec2Element(texcoord_data.data(), texcoord_stride, i);
    }

    return geometry_proto;
}

std::optional<SceneEntity> LoadSceneEntity(tinygltf::Primitive const &primitive,
                                           SceneEntityName const &name,
                                           tinygltf::Model const &model, mat44 const &transform,
                                           ResourceAccessor *resource_accessor) {
    // Materializes resources into the form workable by the rest of the system.
    std::optional<GeometryProto> geometry_proto = LoadGeometry(primitive, model);
    if (!geometry_proto.has_value()) {
        return std::nullopt;
    }

    resource_accessor->AddGeometry(*geometry_proto, /*temporary=*/false);

    // Constructs a scene entity and attaches resources to it.
    SceneEntity scene_entity(name);
    scene_entity.bounding_box = BoundingBoxOf(*geometry_proto);

    SceneEntityResources::Lod *lod = scene_entity.resources.add_lods();
    lod->set_apply_after_distance(0.0f);
    lod->set_geometry_id(geometry_proto->id());

    SceneEntitySetTransform(transform, &scene_entity);

    return scene_entity;
}

void LoadSceneEntities(tinygltf::Mesh const &mesh, tinygltf::Model const &model,
                       mat44 const &transform, SceneObject *scene_object,
                       ResourceAccessor *resource_accessor) {
    for (unsigned i = 0; i < mesh.primitives.size(); ++i) {
        SceneEntityName name = mesh.name + '[' + std::to_string(i) + ']';

        std::optional<SceneEntity> scene_entity =
            LoadSceneEntity(mesh.primitives[i], name, model, transform, resource_accessor);
        if (!scene_entity.has_value()) {
            continue;
        }

        scene_object->AddSceneEntityChild(*scene_entity);
    }
}

std::optional<SceneObject> LoadSceneObject(tinygltf::Node const &node, tinygltf::Model const &model,
                                           mat44 const &transform,
                                           ResourceAccessor *resource_accessor) {
    SceneObject scene_object(node.name);

    if (node.mesh >= 0) {
        tinygltf::Mesh const &mesh = model.meshes[node.mesh];

        LoadSceneEntities(mesh, model, transform, &scene_object, resource_accessor);
        if (!scene_object.HasSceneEntityChildren()) {
            // Prunes empty node.
            return std::nullopt;
        }

        return scene_object;
    }

    for (auto child_index : node.children) {
        tinygltf::Node child_node = model.nodes[child_index];

        mat44 child_transform = transform * NodeTransform(child_node);

        std::optional<SceneObject> child_object =
            LoadSceneObject(child_node, model, child_transform, resource_accessor);
        if (!child_object.has_value()) {
            continue;
        }

        scene_object.AddSceneObjectChild(*child_object);
    }

    if (!scene_object.HasSceneObjectChildren()) {
        // Prunes empty node.
        return std::nullopt;
    }

    return scene_object;
}

} // namespace

std::vector<SceneObject> LoadFromGltf(std::string const &gltf_file_path,
                                      ResourceAccessor *resource_accessor) {
    tinygltf::TinyGLTF loader;

    bool successful = false;
    std::string err;
    std::string warn;
    tinygltf::Model model;

    if (BinaryGltf(gltf_file_path)) {
        successful = loader.LoadBinaryFromFile(&model, &err, &warn, gltf_file_path);
    } else {
        successful = loader.LoadASCIIFromFile(&model, &err, &warn, gltf_file_path);
    }

    if (!successful) {
        BOOST_LOG_TRIVIAL(warning)
            << "LoadFromGltf(): file=[" << gltf_file_path << "], warning=[" << warn << "]";
        BOOST_LOG_TRIVIAL(error) << "LoadFromGltf(): file=[" << gltf_file_path << "], error=["
                                 << err << "]";
        return std::vector<SceneObject>();
    }

    std::vector<SceneObject> scene_objects;
    for (auto node_index : model.scenes[model.defaultScene].nodes) {
        tinygltf::Node node = model.nodes[node_index];

        mat44 node_transform = NodeTransform(node);

        std::optional<SceneObject> scene_object =
            LoadSceneObject(node, model, node_transform, resource_accessor);
        if (!scene_object.has_value()) {
            continue;
        }

        scene_objects.push_back(*scene_object);
    }

    return scene_objects;
}

} // namespace e8
