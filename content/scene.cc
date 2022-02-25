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

#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/tensor.h"
#include "content/common.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene.h"
#include "content/scene_entity.h"
#include "content/scene_linear.h"
#include "content/scene_object.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

SceneInterface::SceneInterface(std::string const &name) : id(GenerateUuid()), name(name) {}

SceneInterface::SceneInterface(SceneProto const &proto) : id(proto.id()), name(proto.name()) {
    for (auto const &scene_object : proto.objects()) {
        this->AddSceneObject(scene_object);
    }

    for (unsigned i = 0; i < 3; ++i) {
        background_color_(i) = proto.background_color(i);
    }
}

SceneInterface::~SceneInterface() {}

bool SceneInterface::AddSceneObject(SceneObject const &scene_object) {
    return scene_objects_.insert(std::make_pair(scene_object.id(), scene_object)).second;
}

bool SceneInterface::DeleteSceneObject(SceneObjectId const &id) {
    return 1 == scene_objects_.erase(id);
}

std::unordered_map<SceneObjectId, SceneObject> const &SceneInterface::AllSceneObjects() const {
    return scene_objects_;
}

void SceneInterface::UpdateBackgroundColor(vec3 const &color) { background_color_ = color; }

vec3 SceneInterface::BackgroundColor() const { return background_color_; }

SceneProto SceneInterface::_ToBaseProto() const {
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

std::unique_ptr<SceneInterface> ToScene(SceneProto const &proto) {
    switch (proto.structure_type()) {
    case SceneProto::LINEAR: {
        return std::make_unique<LinearScene>(proto);
    }
    default: {
        // TODO: Creates octree scene when it's implemented.
        assert(false);
    }
    }
}

} // namespace e8
