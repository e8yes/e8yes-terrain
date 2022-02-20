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

#ifndef ISLANDS_CONTENT_SCENE_LINEAR_H
#define ISLANDS_CONTENT_SCENE_LINEAR_H

#include <vector>

#include "content/entity.h"
#include "content/scene.h"

namespace e8 {

/**
 * @brief The LinearScene class A linear (flat) scene structure.
 */
class LinearScene : public SceneInterface {
  public:
    LinearScene();
    ~LinearScene() override;

    /**
     * @brief AddEntity Adding entity requires O(1) time.
     */
    bool AddEntity(SceneEntity const &entity) override;

    /**
     * @brief DeleteEntity Deleting entity requires O(n) time, where n is the number of entity in
     * the scene.
     */
    bool DeleteEntity(SceneEntityId const &id) override;

    /**
     * @brief FindEntity Finding entity by ID requires O(n) time, where n is the number of entity in
     * the scene.
     */
    SceneEntity const *FindEntity(SceneEntityId const &id) const override;

    /**
     * @brief QueryEntities Querying entity requires O(n) time, where n is the number of entity in
     * the scene.
     */
    std::vector<SceneEntity const *> QueryEntities(QueryFn query_fn) const override;

  private:
    std::vector<SceneEntity> entities_;
};

} // namespace e8

#endif // ISLANDS_CONTENT_SCENE_LINEAR_H
