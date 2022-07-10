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

#ifndef ISLANDS_CONTENT_STRUCTURE_H
#define ISLANDS_CONTENT_STRUCTURE_H

#include <vector>

#include "common/tensor.h"
#include "content/scene_entity.h"

namespace e8 {

/**
 * @brief The SceneEntityStructureInterface class It stores scene entities in data structures which
 * support efficient addition, removal and querying.
 */
class SceneEntityStructureInterface {
   public:
    SceneEntityStructureInterface();
    virtual ~SceneEntityStructureInterface();

    SceneEntityStructureInterface(SceneEntityStructureInterface const &) = delete;

    /**
     * @brief AddEntity Adds a new entity to the structure.
     *
     * @param entity The entity to be added.
     */
    virtual void AddEntity(SceneEntity const *entity) = 0;

    /**
     * @brief DeleteEntity Deletes a scene entity from the structure.
     */
    virtual void DeleteEntity(SceneEntity const *entity) = 0;

    /**
     * @brief Update Marks that the specified entity has been edited (different bounding box or
     * transformation). Updates the internal structure, if needed, to reflect the change correctly.
     * This function is typically fast for any structure implementation, but the internal structure
     * may become suboptimal. Uses the Optimize() call to return it to an optimal state when some
     * amount of individual Update() accumulates.
     */
    virtual void Update(SceneEntity const *entity) = 0;

    /**
     * @brief Optimize Rebuilds/optimizes the internal structure, if needed, for all the scene
     * entities that has been added, deleted or updated.
     */
    virtual void Optimize() = 0;

    // Defines a query function which judges upon the bounding box to determine whether the content
    // in the box should be included in the result list.
    using QueryFn = std::function<bool(aabb const &bounding_box, mat44 const &transform)>;

    /**
     * @brief QueryEntities Selects the entities that satisfy the query function.
     *
     * @param query_fn See above for its definition.
     * @return An array of entities selected.
     */
    virtual std::vector<SceneEntity const *> QueryEntities(QueryFn query_fn) const = 0;
};

}  // namespace e8

#endif  // ISLANDS_CONTENT_STRUCTURE_H
