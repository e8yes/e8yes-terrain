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
#include <vector>

#include "content/structure.h"
#include "renderer/basic/projection.h"
#include "renderer/query/collection.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/query/query_fn.h"
#include "resource/accessor.h"

namespace e8 {

DrawableCollection::DrawableCollection(SceneEntityStructureInterface const& entities,
                                       ResourceAccessor* resource_accessor)
    : entities_(entities), resource_accessor_(resource_accessor) {
    assert(resource_accessor != nullptr);
}

DrawableCollection::~DrawableCollection() {}

std::vector<DrawableInstance> DrawableCollection::ObservableGeometries(
    PerspectiveProjection const& projection, ResourceLoadingOption const& loading_option) {
    std::vector<SceneEntity const*> scene_entities = entities_.QueryEntities(QueryAllSceneEntities);
    return ToDrawables(scene_entities, /*viewer_location=*/projection.Location(), loading_option,
                       resource_accessor_);
}

}  // namespace e8
