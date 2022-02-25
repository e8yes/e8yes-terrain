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
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

Scene::ReadAccess::ReadAccess(std::shared_mutex *mu) : mu_(mu) { mu_->lock_shared(); }

Scene::ReadAccess::ReadAccess(ReadAccess &&other) { mu_ = std::move(other.mu_); }

Scene::ReadAccess::~ReadAccess() { mu_->unlock_shared(); }

Scene::WriteAccess::WriteAccess(std::shared_mutex *mu) : mu_(mu) { mu_->lock(); }

Scene::WriteAccess::WriteAccess(WriteAccess &&other) { mu_ = std::move(other.mu_); }

Scene::WriteAccess::~WriteAccess() { mu_->unlock(); }

Scene::Scene(SceneProto::StructureType structure_type, std::string const &name)
    : id(GenerateUuid()), name(name) {
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

Scene::Scene(SceneProto const &proto) : id(proto.id()), name(proto.name()) {
    for (auto const &scene_object : proto.objects()) {
        this->AddSceneObject(scene_object);
    }

    std::vector<SceneEntity> entities = e8::ToSceneEntities(proto.entities());
    for (auto const &entity : entities) {
        entity_structure_->AddEntity(&entity);
    }
    entity_structure_->Build();

    for (unsigned i = 0; i < 3; ++i) {
        background_color_(i) = proto.background_color(i);
    }
}

Scene::~Scene() {}

Scene::ReadAccess Scene::GainReadAccess() { return ReadAccess(&mu_); }

Scene::WriteAccess Scene::GainWriteAccess() { return WriteAccess(&mu_); }

bool Scene::AddSceneObject(SceneObject const &scene_object) {
    return scene_objects_.insert(std::make_pair(scene_object.id(), scene_object)).second;
}

bool Scene::DeleteSceneObject(SceneObjectId const &id) { return 1 == scene_objects_.erase(id); }

std::unordered_map<SceneObjectId, SceneObject> const &Scene::AllSceneObjects() const {
    return scene_objects_;
}

void Scene::UpdateBackgroundColor(vec3 const &color) { background_color_ = color; }

vec3 Scene::BackgroundColor() const { return background_color_; }

SceneProto Scene::ToProto() const {
    SceneProto proto;

    proto.set_id(id);
    proto.set_name(name);

    for (auto const &[id, scene_object] : scene_objects_) {
        *proto.add_objects() = scene_object;
    }

    proto.mutable_background_color()->Resize(/*new_size=*/3, /*value=*/0.0f);
    for (unsigned i = 0; i < 3; ++i) {
        proto.set_background_color(/*index=*/i, background_color_(i));
    }

    return proto;
}

} // namespace e8
