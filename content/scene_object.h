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

#include <list>
#include <string>

#include "content/common.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene_entity.h"
#include "resource/common.h"

namespace e8 {

// Uniquely identifies a scene object.
using SceneObjectId = Uuid;

// Represents a descriptive human readable name of a scene object.
using SceneObjectName = std::string;

/**
 * @brief The SceneObject struct It logically groups a set of entities into a hierarchy (tree).
 */
struct SceneObject {
    /**
     * @brief SceneObject Creates an empty scene object.
     *
     * @param name A descriptive human readable name for the object.
     * @param procedural_object_id If not null, it indicates this scene object is generated by a
     * procedural process referenced by the ID.
     */
    SceneObject(SceneObjectName const &name,
                ProceduralObjectId const &procedural_object_id = kNullUuid);

    /**
     * @brief SceneObject Reconstructs a scene object from a protobuf object.
     */
    SceneObject(SceneObjectProto const &proto);

    ~SceneObject();

    /**
     * @brief Procedural Checks if the scene object is created by a procedural process.
     */
    bool Procedural() const;

    /**
     * @brief SceneObjectChildren Checks if the scene object's children are scene objects (internal
     * nodes).
     */
    bool HasSceneObjectChildren() const;

    /**
     * @brief SceneEntityChildren Checks if the scene object's children are scene entities
     * (external nodes).
     */
    bool HasSceneEntityChildren() const;

    /**
     * @brief AddSceneObjectChild Adds a scene object as its child. Note, a scene object's children
     * must either be scene objects or scene entities. Adding a mixture will result in failure. In
     * addition, if the parent is procedural, all children must as well be procedural.
     */
    void AddSceneObjectChild(SceneObject const &scene_object);

    /**
     * @brief AddSceneEntityChild Adds a scene entity as its child. Note, a scene object's children
     * must either be scene objects or scene entities. Adding a mixture will result in failure.
     */
    void AddSceneEntityChild(SceneEntity const &scene_entity);

    /**
     * @brief ToProto Turns the object into a protobuf message.
     */
    SceneObjectProto ToProto() const;

    // ID of this scene object.
    SceneObjectId id;

    // Human readable name of this scene object.
    SceneObjectName name;

    // If this field isn't null, it indicates that this scene object is generated procedural
    // process. The process is fully specified by the procedural object referenced by this ID.
    ProceduralObjectId procedural_object_id;

    // Children must either be a list of scene objects or scene entities.
    std::list<SceneObject> child_scene_objects;
    std::list<SceneEntity> child_scene_entities;
};

} // namespace e8

#endif // ISLANDS_CONTENT_SCENE_OBJECT_H
