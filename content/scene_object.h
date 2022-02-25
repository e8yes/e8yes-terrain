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

#ifndef ISLANDS_CONTENT_SCENE_OBJECT_H
#define ISLANDS_CONTENT_SCENE_OBJECT_H

#include <string>
#include <vector>

#include "content/proto/scene_object.pb.h"
#include "content/scene_entity.h"

namespace e8 {

// Uniquely identifies a scene object.
using SceneObjectId = std::string;

// Represents a descriptive human readable name of a scene object.
using SceneObjectName = std::string;

/**
 * @brief CreateSceneObject Creates an empty scene object. All scene object should be created here.
 */
SceneObject CreateSceneObject(SceneObjectName const &name);

/**
 * @brief SceneObjectProcedural Checks if the scene object is created by a procedural process.
 */
bool SceneObjectProcedural(SceneObject const &scene_object);

/**
 * @brief SceneObjectExternalChildren Checks if the scene object's children are scene entities
 * (external nodes).
 */
bool SceneObjectExternalChildren(SceneObject const &scene_object);

/**
 * @brief SceneObjectInternalChild Checks if the scene object's children are scene objects (internal
 * nodes).
 */
bool SceneObjectInternalChildren(SceneObject const &scene_object);

/**
 * @brief SceneObjectAddChild Adds a scene object as its child. Note, a scene object's children must
 * either be scene objects or scene entities. Adding a mixture will result in failure.
 */
void SceneObjectAddChild(SceneObjectId const &scene_object_id, SceneObject *scene_object);

/**
 * @brief SceneObjectAddEntity Adds a scene entity as its child. Note, a scene object's children
 * must either be scene objects or scene entities. Adding a mixture will result in failure.
 */
void SceneObjectAddEntity(SceneEntityId const &scene_entity_id, SceneObject *scene_object);

} // namespace e8

#endif // ISLANDS_CONTENT_SCENE_OBJECT_H
