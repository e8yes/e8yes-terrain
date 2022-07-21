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
#include <cmath>
#include <vector>

#include "common/tensor.h"
#include "content/proto/light_source.pb.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include "renderer/query/light_source.h"
#include "resource/common.h"

namespace e8 {
namespace {

float const kNearClip = 0.1f;
float const kIntensityCutOff = 0.05f;
float const kParallelThreshold = 1e-2f;

LightSource ToViewSpace(LightSource const &light_source, mat44 const &view_model) {
    LightSource view_space_light_source = light_source;

    switch (light_source.model_case()) {
    case LightSource::ModelCase::kSunLight: {
        vec4 direction = ToVec3(light_source.sun_light().direction()).homo(0.0f);
        direction = view_model * direction;

        *view_space_light_source.mutable_sun_light()->mutable_direction() =
            ToProto(direction.trunc());
        break;
    }
    case LightSource::ModelCase::kPointLight: {
        vec4 position = ToVec3(light_source.point_light().position()).homo(1.0f);
        position = view_model * position;

        *view_space_light_source.mutable_point_light()->mutable_position() =
            ToProto(position.trunc());
        break;
    }
    case LightSource::ModelCase::kSpotLight: {
        vec4 position = ToVec3(light_source.spot_light().position()).homo(1.0f);
        vec4 direction = ToVec3(light_source.spot_light().direction()).homo(0.0f);
        position = view_model * position;
        direction = view_model * direction;

        *view_space_light_source.mutable_spot_light()->mutable_position() =
            ToProto(position.trunc());
        *view_space_light_source.mutable_spot_light()->mutable_direction() =
            ToProto(direction.trunc());
        break;
    }
    default: {
        assert(false);
    }
    }

    return view_space_light_source;
}

void ComputeSpotLightBasis(SpotLight const &spot_light, mat44 const &view_model, vec3 *right,
                           vec3 *down, vec3 *back) {
    // Constructs a basis in the spot light's local space.
    *back = ToVec3(spot_light.direction());

    vec3 priming_vector{0.0f, 0.0f, 1.0f};
    float cos_back_100 = back->inner(priming_vector);
    if (1.0f - std::abs(cos_back_100) < kParallelThreshold) {
        priming_vector = vec3{0.0f, 1.0f, 0.0f};
    }

    *right = priming_vector.outer(*back);
    *down = back->outer(*right);

    // Transforms the basis to the view space.
    *right = (view_model * right->homo(0.0f)).trunc();
    *down = (view_model * down->homo(0.0f)).trunc();
    *back = (view_model * back->homo(0.0f)).trunc();

    // Restores orthogonality which was lost through numerical error incurred from the
    // transformation.
    *down = back->outer(*right);
    *back = right->outer(*down);
}

SpotLightVolume ComputeSpotLightRegion(SpotLight const &spot_light, mat44 const &view_model) {
    float intensity_0 = ToVec3(spot_light.intensity()).sum();
    float far_clip = std::sqrt(intensity_0 / kIntensityCutOff);

    float half_fov = deg2rad(spot_light.outer_cone_angle() / 2.0f);
    float width = 2.0f * kNearClip * std::tan(half_fov);
    float height = width;

    vec4 location = view_model * ToVec3(spot_light.position()).homo(1.0f);

    vec3 right;
    vec3 down;
    vec3 back;
    ComputeSpotLightBasis(spot_light, view_model, &right, &down, &back);

    PerspectiveProjection projection(/*near_clip=*/kNearClip, far_clip, width, height,
                                     location.trunc(), right, down, back);

    // TODO: Determines the light source's 0LOD through distance and importance.
    return SpotLightVolume(LightSourceLod::LSL_LEVEL0, projection);
}

LightVolume ComputeLightRegions(LightSource const &light_source, mat44 const &view_model) {
    LightVolume light_regions;

    switch (light_source.model_case()) {
    case LightSource::ModelCase::kSunLight: {
        // TODO: Compute regions for sun lights.
        break;
    }
    case LightSource::ModelCase::kPointLight: {
        break;
    }
    case LightSource::ModelCase::kSpotLight: {
        light_regions.spot_light_region =
            ComputeSpotLightRegion(light_source.spot_light(), view_model);
        break;
    }
    default: {
        assert(false);
    }
    }

    return light_regions;
}

} // namespace

SpotLightVolume::SpotLightVolume(LightSourceLod lod, PerspectiveProjection projection)
    : lod(lod), projection(projection) {}

std::vector<LightSourceInstance>
ToLightSources(std::vector<SceneEntity const *> const &scene_entities,
               ProjectionInterface const &camera_projection) {
    std::vector<LightSourceInstance> instances;

    for (auto entity : scene_entities) {
        if (!entity->light_source.has_value()) {
            continue;
        }

        LightSourceInstance instance;
        instance.id = entity->id;

        mat44 view_model = camera_projection.ViewTransform() * entity->transform;
        instance.light_source = ToViewSpace(*entity->light_source, view_model);
        instance.light_volume = ComputeLightRegions(*entity->light_source, view_model);

        instances.push_back(instance);
    }

    return instances;
}

} // namespace e8
