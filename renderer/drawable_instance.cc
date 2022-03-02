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

#include "renderer/drawable_instance.h"
#include "common/tensor.h"
#include "content/proto/geometry.pb.h"
#include "content/scene_entity.h"

namespace e8 {
namespace {

int SelectLod(vec3 const &viewer_location, SceneEntity const *scene_entity) {
    float entity_distance = (scene_entity->bounding_box.centroid() - viewer_location).norm();

    for (int i = 0; i < scene_entity->geometry_lod_instance->min_distances_size(); ++i) {
        if (entity_distance >= scene_entity->geometry_lod_instance->min_distances(i)) {
            return i;
        }
    }

    return -1;
}

} // namespace

std::vector<DrawableInstance> ToDrawables(std::vector<SceneEntity const *> const &scene_entities,
                                         vec3 const &viewer_location) {
    std::vector<DrawableInstance> result;

    for (auto const scene_entity : scene_entities) {
        int lod_selection = SelectLod(viewer_location, scene_entity);
        if (lod_selection == -1) {
            continue;
        }

        DrawableInstance drawable;
        drawable.geometry = &scene_entity->geometry_lod_instance->geometry_lod(lod_selection);
        drawable.transform = &scene_entity->transform;

        result.push_back(drawable);
    }

    return result;
}

} // namespace e8
