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
#include "renderer/drawable/collection.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/drawable/query_fn.h"
#include "resource/accessor.h"

namespace e8 {

DrawableCollection::DrawableCollection(SceneEntityStructureInterface const &entities,
                                       ResourceAccessor *resource_accessor)
    : entities_(entities), resource_accessor_(resource_accessor) {
    assert(resource_accessor != nullptr);
}

DrawableCollection::~DrawableCollection() {}

std::vector<DrawableInstance>
DrawableCollection::ObservableGeometries(PerspectiveProjection const &projection,
                                         ResourceLoadingOption const &loading_option) {
    std::vector<SceneEntity const *> scene_entities =
        entities_.QueryEntities(QueryAllSceneEntities);
    return ToDrawables(scene_entities, /*viewer_location=*/projection.Location(), loading_option,
                       resource_accessor_);
}

std::vector<LightSourceInstance>
DrawableCollection::ObservableLightSources(PerspectiveProjection const &projection) {
    std::vector<SceneEntity const *> scene_entities =
        entities_.QueryEntities(QueryAllSceneEntities);

    SceneEntity light1("light1");
    light1.id = 10;
    SceneEntitySetTransform(mat44_identity(), &light1);

    LightSource light_source1;
    *light_source1.mutable_spot_light()->mutable_position() = ToProto(vec3{0, 0, 9});
    *light_source1.mutable_spot_light()->mutable_direction() = ToProto(vec3{0, 0, -1});
    *light_source1.mutable_spot_light()->mutable_intensity() =
        ToProto(vec3{1.0f, 1.0f, 1.4f} * 100.0f);
    light_source1.mutable_spot_light()->set_inner_cone_angle(90.0f);
    light_source1.mutable_spot_light()->set_outer_cone_angle(120.0f);

    light1.light_source = light_source1;

    scene_entities.push_back(&light1);

    return ToLightSources(scene_entities, projection);
}

} // namespace e8
