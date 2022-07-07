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

#ifndef ISLANDS_CONTENT_STRUCTURE_LINEAR_H
#define ISLANDS_CONTENT_STRUCTURE_LINEAR_H

#include <vector>

#include "common/tensor.h"
#include "content/scene_entity.h"
#include "content/structure.h"

namespace e8 {

/**
 * @brief The LinearSceneEntityContainer class A linear (flat) scene entity container structure.
 */
class LinearSceneEntityStructure : public SceneEntityStructureInterface {
   public:
    /**
     * @brief LinearScene Constructs a empty linear scene.
     */
    LinearSceneEntityStructure();
    ~LinearSceneEntityStructure() override;

    /**
     * @brief AddEntity Adding entity requires O(1) time.
     */
    void AddEntity(SceneEntity const *entity) override;

    /**
     * @brief DeleteEntity Deleting entity requires O(n) time, where n is the number of entity in
     * the scene.
     */
    void DeleteEntity(SceneEntity const *entity) override;

    /**
     * @brief Update It does nothing.
     */
    void Update(SceneEntity const *entity) override;

    /**
     * @brief Optimize It does nothing.
     */
    void Optimize() override;

    /**
     * @brief QueryEntities Querying entity requires O(n) time, where n is the number of entity in
     * the scene.
     */
    std::vector<SceneEntity const *> QueryEntities(QueryFn query_fn) override;

    void InvalidateQueryCache() override;

   private:
    std::vector<SceneEntity const *> entities_;
};

}  // namespace e8

#endif  // ISLANDS_CONTENT_STRUCTURE_LINEAR_H
