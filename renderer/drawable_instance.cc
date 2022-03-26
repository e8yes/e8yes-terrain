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

#include <memory>
#include <vector>

#include "common/tensor.h"
#include "content/scene_entity.h"
#include "renderer/drawable_instance.h"
#include "resource/accessor.h"
#include "resource/geometry.h"

namespace e8 {
namespace {

Geometry *LoadGeometry(SceneEntity const &scene_entity, vec3 const &viewer_location,
                       ResourceAccessor *resource_accessor) {
    std::shared_ptr<GeometryLod> geometry_lod =
        resource_accessor->LoadGeometry(scene_entity.geometry_id);
    if (geometry_lod == nullptr) {
        return nullptr;
    }

    float entity_distance = (scene_entity.bounding_box.centroid() - viewer_location).norm();

    for (unsigned i = 0; i < geometry_lod->lod_min_distances.size(); ++i) {
        if (entity_distance >= geometry_lod->lod_min_distances[i]) {
            return &geometry_lod->lod[i];
        }
    }

    return nullptr;
}

} // namespace

std::vector<DrawableInstance> ToDrawables(std::vector<SceneEntity const *> const &scene_entities,
                                          vec3 const &viewer_location,
                                          ResourceAccessor *resource_accessor) {
    std::vector<DrawableInstance> instances;

    for (auto const scene_entity : scene_entities) {
        Geometry *geometry = LoadGeometry(*scene_entity, viewer_location, resource_accessor);
        if (geometry == nullptr) {
            continue;
        }

        DrawableInstance drawable;
        drawable.geometry = geometry;
        drawable.transform = &scene_entity->transform;

        instances.push_back(drawable);
    }

    return instances;
}

} // namespace e8
