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
#include <functional>
#include <map>
#include <shared_mutex>
#include <string>

#include "common/tensor.h"
#include "content/common.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "content/structure.h"
#include "content/structure_linear.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

void CollectSceneEntityToStructure(SceneObject const &scene_object,
                                   SceneEntityStructureInterface *structure) {
    if (scene_object.HasSceneEntityChildren()) {
        for (auto const &child_entity : scene_object.child_scene_entities) {
            structure->AddEntity(&child_entity);
        }
        return;
    }

    for (auto const &child_object : scene_object.child_scene_objects) {
        CollectSceneEntityToStructure(child_object, structure);
    }
}

} // namespace

Scene::ReadAccess::ReadAccess(std::shared_mutex *mu) : mu_(mu) { mu_->lock_shared(); }

Scene::ReadAccess::ReadAccess(ReadAccess &&other) { mu_ = std::move(other.mu_); }

Scene::ReadAccess::~ReadAccess() { mu_->unlock_shared(); }

Scene::WriteAccess::WriteAccess(std::shared_mutex *mu) : mu_(mu) { mu_->lock(); }

Scene::WriteAccess::WriteAccess(WriteAccess &&other) { mu_ = std::move(other.mu_); }

Scene::WriteAccess::~WriteAccess() { mu_->unlock(); }

Scene::Scene(SceneProto::StructureType structure_type, std::string const &name)
    : id(GenerateUuid()), name(name), structure_type(structure_type) {
    this->CreateSceneEntityStructure();
}

Scene::Scene(SceneProto const &proto)
    : id(proto.id()), name(proto.name()), structure_type(proto.structure_type()) {
    root_scene_objects_ = ToSceneObjects(proto.objects());

    this->CreateSceneEntityStructure();
    for (auto const &[_, root_scene_object] : root_scene_objects_) {
        CollectSceneEntityToStructure(root_scene_object, entity_structure_.get());
    }
    entity_structure_->Build();

    background_color_ = ToVec3(proto.background_color());
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

bool Scene::AddRootSceneObject(SceneObject const &scene_object) {
    return root_scene_objects_.insert(std::make_pair(scene_object.id, scene_object)).second;
}

bool Scene::DeleteRootSceneObject(SceneObjectId const &id) {
    return 1 == root_scene_objects_.erase(id);
}

std::map<SceneObjectId, SceneObject> const &Scene::AllRootSceneObjects() const {
    return root_scene_objects_;
}

void Scene::UpdateBackgroundColor(vec3 const &color) { background_color_ = color; }

vec3 Scene::BackgroundColor() const { return background_color_; }

SceneProto Scene::ToProto() const {
    SceneProto proto;

    proto.set_id(id);
    proto.set_name(name);
    proto.set_structure_type(structure_type);
    *proto.mutable_objects() = e8::ToProto(root_scene_objects_);
    *proto.mutable_background_color() = e8::ToProto(background_color_);

    return proto;
}

} // namespace e8
