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
#include <google/protobuf/map.h>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>

#include "content/common.h"
#include "content/drawable.h"
#include "content/proto/drawable.pb.h"
#include "content/proto/entity.pb.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"

namespace e8 {

void CollectDrawables(SceneObject const &scene_object,
                      google::protobuf::Map<DrawableId, DrawableLod> *drawables) {
    if (scene_object.HasSceneEntityChildren()) {
        for (auto const &child_entity : scene_object.child_scene_entities) {
            if (child_entity.drawable_lod_instance == nullptr) {
                continue;
            }

            (*drawables)[child_entity.drawable_lod_instance->id()] =
                *child_entity.drawable_lod_instance;
        }

        return;
    }

    for (auto const &child_object : scene_object.child_scene_objects) {
        CollectDrawables(child_object, drawables);
    }
}

SceneObject::SceneObject(SceneObjectName const &name,
                         std::optional<ProceduralObjectId> const &procedural_object_id)
    : id(GenerateUuid()), name(name), procedural_object_id(procedural_object_id) {}

SceneObject::SceneObject(
    SceneObjectProto const &proto,
    std::unordered_map<DrawableId, std::shared_ptr<DrawableLod>> const &drawables)
    : id(proto.id()), name(proto.name()) {
    if (!proto.procedural_object_id().empty()) {
        procedural_object_id = proto.procedural_object_id();
    }

    if (proto.has_scene_object_children()) {
        for (auto const &child : proto.scene_object_children().scene_objects()) {
            this->AddSceneObjectChild(SceneObject(child, drawables));
        }
    }

    if (proto.has_scene_entity_children()) {
        for (auto const &child : proto.scene_entity_children().scene_entities()) {
            this->AddSceneEntityChild(SceneEntity(child, drawables));
        }
    }
}

SceneObject::~SceneObject() {}

bool SceneObject::Procedural() const { return procedural_object_id.has_value(); }

bool SceneObject::HasSceneObjectChildren() const { return !child_scene_objects.empty(); }

bool SceneObject::HasSceneEntityChildren() const { return !child_scene_entities.empty(); }

void SceneObject::AddSceneObjectChild(SceneObject const &scene_object) {
    assert(!this->HasSceneEntityChildren());
    child_scene_objects.push_back(scene_object);
}

void SceneObject::AddSceneEntityChild(SceneEntity const &scene_entity) {
    assert(!this->HasSceneObjectChildren());
    child_scene_entities.push_back(scene_entity);
}

SceneObjectProto SceneObject::ToProto() const {
    SceneObjectProto proto;

    proto.set_id(id);
    proto.set_name(name);
    if (procedural_object_id.has_value()) {
        proto.set_procedural_object_id(*procedural_object_id);
    }

    if (this->HasSceneObjectChildren()) {
        for (auto const &scene_object : child_scene_objects) {
            *proto.mutable_scene_object_children()->add_scene_objects() = scene_object.ToProto();
        }
    } else {
        for (auto const &scene_entity : child_scene_entities) {
            *proto.mutable_scene_entity_children()->add_scene_entities() = scene_entity.ToProto();
        }
    }

    return proto;
}

SceneObjectCollection ToProto(std::map<SceneObjectId, SceneObject> const &scene_objects) {
    SceneObjectCollection proto;

    for (auto const &[_, scene_object] : scene_objects) {
        *proto.add_scene_objects() = scene_object.ToProto();
        CollectDrawables(scene_object, proto.mutable_drawables());
    }

    return proto;
}

std::map<SceneObjectId, SceneObject> ToSceneObjects(SceneObjectCollection const &proto) {
    std::unordered_map<DrawableId, std::shared_ptr<DrawableLod>> drawables;
    for (auto const &[id, drawable] : proto.drawables()) {
        drawables[id] = std::make_shared<DrawableLod>(drawable);
    }

    std::map<SceneObjectId, SceneObject> root_scene_objects;
    for (auto scene_object_proto : proto.scene_objects()) {
        root_scene_objects.insert(
            std::make_pair(scene_object_proto.id(), SceneObject(scene_object_proto, drawables)));
    }

    return root_scene_objects;
}

} // namespace e8
