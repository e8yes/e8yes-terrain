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
#include <string>
#include <vector>

#include "content/entity.h"
#include "content/proto/scene.pb.h"
#include "content/scene.h"
#include "content/scene_linear.h"

namespace e8 {

LinearScene::LinearScene(std::string const &name) : SceneInterface(name) {}

LinearScene::LinearScene(SceneProto const &proto)
    : SceneInterface(proto), entities_(ToSceneEntities(proto.entities())) {}

LinearScene::~LinearScene() {}

bool LinearScene::AddEntity(SceneEntity const &entity) {
    auto it = _scene_entity_ids.find(entity.id);
    if (it != _scene_entity_ids.end()) {
        return false;
    }
    _scene_entity_ids.insert(entity.id);

    entities_.push_back(entity);
    return true;
}

bool LinearScene::DeleteEntity(SceneEntityId const &id) {
    if (1 != _scene_entity_ids.erase(id)) {
        return false;
    }

    auto it = std::find_if(entities_.begin(), entities_.end(),
                           [&id](SceneEntity const &entity) { return entity.id == id; });
    assert(it != entities_.end());
    entities_.erase(it);

    return true;
}

SceneEntity const *LinearScene::FindEntity(SceneEntityId const &id) const {
    auto it = std::find_if(entities_.begin(), entities_.end(),
                           [&id](SceneEntity const &entity) { return entity.id == id; });
    if (it == entities_.end()) {
        return nullptr;
    }

    return &(*it);
}

std::vector<SceneEntity const *> LinearScene::QueryEntities(QueryFn query_fn) {
    std::vector<SceneEntity const *> result;

    for (auto const &entity : entities_) {
        if (query_fn(entity.bounding_box, entity.transform)) {
            result.push_back(&entity);
        }
    }

    return result;
}

SceneProto LinearScene::ToProto() const {
    SceneProto proto = this->_ToBaseProto();
    proto.set_structure_type(SceneProto::LINEAR);
    *proto.mutable_entities() = e8::ToProto(entities_);
    return proto;
}

} // namespace e8
