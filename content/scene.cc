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
#include <functional>
#include <google/protobuf/repeated_field.h>
#include <map>
#include <memory>
#include <shared_mutex>
#include <string>

#include "common/tensor.h"
#include "content/camera.h"
#include "content/common.h"
#include "content/procedural_object.h"
#include "content/proto/scene.pb.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "content/structure.h"
#include "content/structure_linear.h"
#include "resource/accessor.h"
#include "resource/common.h"

namespace e8 {
namespace {

constexpr char const *kSceneFileName = "scene.spb";

std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>>
ToProceduralObjects(google::protobuf::RepeatedPtrField<ProceduralObjectProto> const &protos) {
    std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>> procedural_objects;
    for (auto const &proto : protos) {
        procedural_objects.insert(std::make_pair(proto.id(), ToProceduralObject(proto)));
    }
    return procedural_objects;
}

std::map<SceneObjectId, SceneObject>
ToSceneObjects(google::protobuf::RepeatedPtrField<SceneObjectProto> const &protos) {
    std::map<SceneObjectId, SceneObject> scene_objects;
    for (auto const &proto : protos) {
        scene_objects.insert(std::make_pair(proto.id(), SceneObject(proto)));
    }
    return scene_objects;
}

google::protobuf::RepeatedPtrField<ProceduralObjectProto> ToProceduralObjectProtos(
    std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>> const
        &procedural_objects) {
    google::protobuf::RepeatedPtrField<ProceduralObjectProto> protos;
    for (auto const &[_, procedural_object] : procedural_objects) {
        *protos.Add() = procedural_object->ToProto();
    }
    return protos;
}

google::protobuf::RepeatedPtrField<SceneObjectProto>
ToSceneObjectProtos(std::map<SceneObjectId, SceneObject> const &scene_objects) {
    google::protobuf::RepeatedPtrField<SceneObjectProto> protos;
    for (auto const &[_, scene_object] : scene_objects) {
        *protos.Add() = scene_object.ToProto();
    }
    return protos;
}

void FillProceduralObject(
    SceneObject *scene_object,
    std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>> const
        &procedural_objects,
    ResourceAccessor *resource_accessor) {
    if (scene_object->HasSceneEntityChildren()) {
        return;
    }

    if (scene_object->Procedural()) {
        // Replaces a procedural node with a generated scene object.
        auto const &procedural_object = procedural_objects.at(scene_object->procedural_object_id);
        *scene_object = procedural_object->ToSceneObject(resource_accessor);
        return;
    }

    for (auto &child_object : scene_object->child_scene_objects) {
        FillProceduralObject(&child_object, procedural_objects, resource_accessor);
    }
}

void CollectSceneEntityToStructure(SceneObject const &scene_object, bool add,
                                   SceneEntityStructureInterface *structure) {
    if (scene_object.HasSceneEntityChildren()) {
        for (auto const &child_entity : scene_object.child_scene_entities) {
            if (add) {
                structure->AddEntity(&child_entity);
            } else {
                structure->DeleteEntity(&child_entity);
            }
        }
        return;
    }

    for (auto const &child_object : scene_object.child_scene_objects) {
        CollectSceneEntityToStructure(child_object, add, structure);
    }
}

} // namespace

Scene::ReadAccess::ReadAccess(std::shared_mutex *mu) : mu_(mu) { mu_->lock_shared(); }

Scene::ReadAccess::ReadAccess(ReadAccess &&other) {
    mu_ = other.mu_;
    other.mu_ = nullptr;
}

Scene::ReadAccess::~ReadAccess() {
    if (mu_ != nullptr) {
        // The lock has not been moved.
        mu_->unlock_shared();
    }
}

Scene::WriteAccess::WriteAccess(std::shared_mutex *mu) : mu_(mu) { mu_->lock(); }

Scene::WriteAccess::WriteAccess(WriteAccess &&other) {
    mu_ = other.mu_;
    other.mu_ = nullptr;
}

Scene::WriteAccess::~WriteAccess() {
    if (mu_ != nullptr) {
        // The lock has not been moved.
        mu_->unlock();
    }
}

Scene::Scene(SceneProto::StructureType structure_type, std::string const &name)
    : id(GenerateUuid()), name(name), structure_type(structure_type), camera(DefaultCamera()) {
    this->CreateSceneEntityStructure();
}

Scene::Scene(SceneProto const &proto, ResourceAccessor *resource_accessor)
    : id(proto.id()), name(proto.name()), structure_type(proto.structure_type()) {
    procedural_objects_ = ToProceduralObjects(proto.precedural_objects());

    root_scene_objects_ = ToSceneObjects(proto.scene_objects());
    for (auto &[_, root_scene_object] : root_scene_objects_) {
        FillProceduralObject(&root_scene_object, procedural_objects_, resource_accessor);
    }

    this->CreateSceneEntityStructure();
    for (auto const &[_, root_scene_object] : root_scene_objects_) {
        CollectSceneEntityToStructure(root_scene_object, /*add=*/true, entity_structure_.get());
    }
    entity_structure_->Optimize();

    background_color = ToVec3(proto.background_color());
    camera = proto.camera();
}

Scene::~Scene() {}

void Scene::CreateSceneEntityStructure() {
    switch (structure_type) {
    case SceneProto::LINEAR: {
        entity_structure_ = std::make_unique<LinearSceneEntityStructure>();
        break;
    }
    case SceneProto::OCTREE: {
        // TODO: Creates an octree scene entity container once it's implemented.
        assert(false);
        break;
    }
    default: {
        assert(false);
    }
    }
}

Scene::ReadAccess Scene::GainReadAccess() { return ReadAccess(&mu_); }

Scene::WriteAccess Scene::GainWriteAccess() { return WriteAccess(&mu_); }

SceneEntityStructureInterface *Scene::SceneEntityStructure() { return entity_structure_.get(); }

bool Scene::AddRootSceneObject(SceneObject const &scene_object) {
    auto const &[it, inserted] =
        root_scene_objects_.insert(std::make_pair(scene_object.id, scene_object));
    if (!inserted) {
        return false;
    }
    CollectSceneEntityToStructure(it->second, /*add=*/true, entity_structure_.get());
    return true;
}

bool Scene::DeleteRootSceneObject(SceneObjectId const &id) {
    auto it = root_scene_objects_.find(id);
    if (it == root_scene_objects_.end()) {
        return false;
    }
    CollectSceneEntityToStructure(it->second, /*add=*/false, entity_structure_.get());
    root_scene_objects_.erase(it);
    return true;
}

bool Scene::AddProceduralObject(std::unique_ptr<ProceduralObjectInterface> &&procedural_object,
                                ResourceAccessor *resource_accessor) {
    auto it = procedural_objects_.find(procedural_object->id);
    if (it != procedural_objects_.end()) {
        return false;
    }
    it = procedural_objects_
             .insert(std::make_pair(procedural_object->id, std::move(procedural_object)))
             .first;
    this->AddRootSceneObject(it->second->ToSceneObject(resource_accessor));
    return true;
}

std::map<SceneObjectId, SceneObject> const &Scene::AllRootSceneObjects() const {
    return root_scene_objects_;
}

SceneProto Scene::ToProto() const {
    SceneProto proto;

    proto.set_id(id);
    proto.set_name(name);
    proto.set_structure_type(structure_type);
    *proto.mutable_background_color() = e8::ToProto(background_color);
    *proto.mutable_camera() = camera;
    *proto.mutable_precedural_objects() = e8::ToProceduralObjectProtos(procedural_objects_);
    *proto.mutable_scene_objects() = e8::ToSceneObjectProtos(root_scene_objects_);

    return proto;
}

std::unique_ptr<Scene> LoadScene(std::filesystem::path const &base_path,
                                 ResourceAccessor *resource_accessor) {
    std::fstream scene_file(base_path / kSceneFileName, std::ios::in | std::ios::binary);
    if (!scene_file.is_open()) {
        return nullptr;
    }

    SceneProto proto;
    if (!proto.ParseFromIstream(&scene_file)) {
        return nullptr;
    }

    return std::make_unique<Scene>(proto, resource_accessor);
}

bool SaveScene(Scene const &scene, std::filesystem::path const &base_path) {
    std::fstream scene_file(base_path / kSceneFileName,
                            std::ios::out | std::ios::trunc | std::ios::binary);
    if (!scene_file.is_open()) {
        return false;
    }

    SceneProto proto = scene.ToProto();
    if (!proto.SerializeToOstream(&scene_file)) {
        return false;
    }

    return true;
}

} // namespace e8
