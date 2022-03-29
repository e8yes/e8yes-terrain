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
#include <optional>
#include <string>
#include <vector>

#include "content/common.h"
#include "content/loader_gltf.h"
#include "content/loader_gltf_geometry.h"
#include "content/loader_gltf_material.h"
#include "content/proto/lod.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "resource/accessor.h"
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

std::optional<SceneEntity> LoadSceneEntity(tinygltf::Primitive const &primitive,
                                           SceneEntityName const &name,
                                           tinygltf::Model const &model, mat44 const &transform,
                                           ResourceAccessor *resource_accessor) {
    // Materializes resources into the form workable by the rest of the system.
    std::optional<GeometryProto> geometry_proto = LoadGeometry(primitive, name, model);
    if (!geometry_proto.has_value()) {
        return std::nullopt;
    }
    resource_accessor->AddGeometry(*geometry_proto, /*temporary=*/false);

    if (primitive.material >= 0) {
        MaterialProto material_proto = LoadMaterial(model.materials[primitive.material], model);
        resource_accessor->AddMaterial(material_proto, /*temporary=*/false);
    }

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
    loader.SetPreserveImageChannels(true);

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
