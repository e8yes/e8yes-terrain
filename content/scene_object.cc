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
#include <string>
#include <vector>

#include "content/common.h"
#include "content/proto/procedural_object.pb.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"

namespace e8 {

SceneObject CreateSceneObject(SceneObjectName const &name) {
    SceneObject object;
    object.set_id(GenerateUuid());
    object.set_name(name);
    return object;
}

bool SceneObjectProcedural(SceneObject const &scene_object) {
    return !scene_object.procedural_object_id().empty();
}

bool SceneObjectExternalChildren(SceneObject const &scene_object) {
    return scene_object.has_scene_entity_children();
}

bool SceneObjectInternalChildren(SceneObject const &scene_object) {
    return scene_object.has_scene_object_children();
}

void SceneObjectAddChild(SceneObjectId const &scene_object_id, SceneObject *scene_object) {
    assert(!SceneObjectExternalChildren(*scene_object));
    scene_object->mutable_scene_object_children()->add_scene_object_ids(scene_object_id);
}

void SceneObjectAddEntity(SceneEntityId const &scene_entity_id, SceneObject *scene_object) {
    assert(!SceneObjectInternalChildren(*scene_object));
    scene_object->mutable_scene_entity_children()->add_scene_entity_ids(scene_entity_id);
}

} // namespace e8
