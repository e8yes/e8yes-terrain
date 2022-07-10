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

#include <algorithm>
#include <cassert>
#include <vector>

#include "common/tensor.h"
#include "content/scene_entity.h"
#include "content/structure.h"
#include "content/structure_linear.h"

namespace e8 {

LinearSceneEntityStructure::LinearSceneEntityStructure() {}

LinearSceneEntityStructure::~LinearSceneEntityStructure() {}

void LinearSceneEntityStructure::AddEntity(SceneEntity const *entity) {
    entities_.push_back(entity);
}

void LinearSceneEntityStructure::DeleteEntity(SceneEntity const *entity) {
    auto it = std::find_if(entities_.begin(), entities_.end(),
                           [entity](SceneEntity const *src) { return src->id == entity->id; });
    assert(it != entities_.end());
    entities_.erase(it);
}

void LinearSceneEntityStructure::Update(SceneEntity const * /*entity*/) {}

void LinearSceneEntityStructure::Optimize() {}

std::vector<SceneEntity const *> LinearSceneEntityStructure::QueryEntities(QueryFn query_fn) const {
    std::vector<SceneEntity const *> result;

    for (auto const &entity : entities_) {
        if (query_fn(entity->bounding_box, entity->transform)) {
            result.push_back(entity);
        }
    }

    return result;
}

}  // namespace e8
