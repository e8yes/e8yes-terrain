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

#include "content/proto/light_source.pb.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include "renderer/query/light_source.h"
#include "resource/common.h"

namespace e8 {

std::vector<LightSourceInstance>
ToLightSources(std::vector<SceneEntity const *> const &scene_entities,
               ProjectionInterface const &camera_projection) {
    std::vector<LightSourceInstance> instances;

    for (auto entity : scene_entities) {
        if (!entity->light_source.has_value()) {
            continue;
        }

        LightSource light_source = *entity->light_source;
        mat44 view_model = camera_projection.ViewTransform() * entity->transform;

        switch (light_source.model_case()) {
        case LightSource::ModelCase::kSunLight: {
            vec4 direction = ToVec3(light_source.mutable_sun_light()->direction()).homo(0.0f);
            direction = view_model * direction;

            *light_source.mutable_sun_light()->mutable_direction() = ToProto(direction);
            break;
        }
        case LightSource::ModelCase::kPointLight: {
            vec4 position = ToVec3(light_source.mutable_point_light()->position()).homo(1.0f);
            position = view_model * position;

            *light_source.mutable_point_light()->mutable_position() = ToProto(position);
            break;
        }
        case LightSource::ModelCase::kSpotLight: {
            vec4 position = ToVec3(light_source.mutable_spot_light()->position()).homo(1.0f);
            vec4 direction = ToVec3(light_source.mutable_spot_light()->direction()).homo(0.0f);
            direction = view_model * direction;
            position = view_model * position;

            *light_source.mutable_spot_light()->mutable_position() = ToProto(position);
            *light_source.mutable_spot_light()->mutable_direction() = ToProto(position);
            break;
        }
        default: {
            assert(false);
        }
        }

        LightSourceInstance instance;
        instance.light_source = light_source;

        instances.push_back(instance);
    }

    return instances;
}

} // namespace e8
