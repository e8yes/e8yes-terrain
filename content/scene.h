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

#ifndef ISLANDS_RENDERER_SCENE_H
#define ISLANDS_RENDERER_SCENE_H

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/tensor.h"
#include "content/entity.h"

namespace e8 {

// Uniquely identifies a scene.
using SceneId = std::string;

/**
 * @brief The SceneInterface class a container for efficient scene object storage and query. This
 * container isn't thread-safe.
 */
class SceneInterface {
  public:
    /**
     * @brief SceneInterface Constructs a scene with a human readable name.
     */
    explicit SceneInterface(std::string const &name);
    virtual ~SceneInterface();

    SceneInterface(SceneInterface const &) = delete;

    /**
     * @brief AddEntity Adds a new entity to the scene.
     *
     * @param entity The entity to be added.
     * @return true only if the entity has not been added to the scene.
     */
    virtual bool AddEntity(SceneEntity const &entity) = 0;

    /**
     * @brief DeleteEntity Deletes a scene entity by its ID. It returns true only when the entity
     * exists.
     */
    virtual bool DeleteEntity(SceneEntityId const &id) = 0;

    /**
     * @brief FindEntity Finds a scene entity by its ID. It return a nullptr if it doesn't exist.
     * When it returns a non-null pointer, it's only valid before the next AddEntity() and
     * DeleteEntity() call.
     */
    virtual SceneEntity const *FindEntity(SceneEntityId const &id) const = 0;

    // Defines a query function which judges upon the bounding box to determine whether the content
    // in the box should be included in the result list.
    using QueryFn = std::function<bool(aabb const &bounding_box, mat44 const &transform)>;

    /**
     * @brief QueryEntities Selects the entities that satisfy the query function. The pointers in
     * the result array is only valid before the next AddEntity() and DeleteEntity() call.
     *
     * @param query_fn See above for its definition.
     * @return An array of entities selected.
     */
    virtual std::vector<SceneEntity const *> QueryEntities(QueryFn query_fn) const = 0;

    /**
     * @brief AddSceneObject Adds a new scene object to the scene if it has not already been added.
     * Otherwise, it will do nothing. The caller must ensures the entities grouped by the scene
     * object must have been added to the scene via the AddEntity() call, or else, this function
     * will fail.
     *
     * @param scene_object The scene object to be moved into the scene.
     * @return true only if the scene object has not been added to the scene.
     */
    bool AddSceneObject(SceneObject const &scene_object);

    /**
     * @brief DeleteSceneObject Deletes a scene object by ID. It returns true if the scene object
     * exists.
     */
    bool DeleteSceneObject(SceneObjectId const &id);

    /**
     * @brief AllSceneObjects Returns a list of all scene objects added to the scene.
     */
    std::unordered_map<SceneObjectId, SceneObject> const &AllSceneObjects() const;

    /**
     * @brief UpdateBackgroundColor Sets a new background color for the scene.
     */
    void UpdateBackgroundColor(vec3 const &color);

    /**
     * @brief BackgroundColor Returns the scene's current background color.
     */
    vec3 BackgroundColor() const;

  public:
    // Id of the scene.
    SceneId id;

    // A descriptive human readable name of the scene.
    std::string name;

  protected:
    std::unordered_set<SceneEntityId> _scene_entity_ids;

  private:
    std::unordered_map<SceneObjectId, SceneObject> scene_objects_;
    vec3 background_color_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_SCENE_H
