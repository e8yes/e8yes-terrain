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
#include "content/drawable.h"
#include "content/entity.h"
#include "content/scene.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

std::mutex gUuidGenMutex;
bool gUuidInitialized = false;
UUID4_STATE_T gUuidGenState;

SceneId GenerateSceneId() {
    std::lock_guard guard(gUuidGenMutex);

    if (!gUuidInitialized) {
        uuid4_seed(&gUuidGenState);
        gUuidInitialized = true;
    }

    UUID4_T uuid;
    uuid4_gen(&gUuidGenState, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return SceneId(uuid_string);
}

} // namespace

SceneInterface::SceneInterface(std::string const &name) : id(GenerateSceneId()), name(name) {}

SceneInterface::~SceneInterface() {}

bool SceneInterface::AddSceneObject(SceneObject const &scene_object) {
    return scene_objects_.insert(std::make_pair(scene_object.id, scene_object)).second;
}

bool SceneInterface::DeleteSceneObject(SceneObjectId const &id) {
    return 1 == scene_objects_.erase(id);
}

std::unordered_map<SceneObjectId, SceneObject> const &SceneInterface::AllSceneObjects() const {
    return scene_objects_;
}

void SceneInterface::UpdateBackgroundColor(vec3 const &color) { background_color_ = color; }

vec3 SceneInterface::BackgroundColor() const { return background_color_; }

} // namespace e8
