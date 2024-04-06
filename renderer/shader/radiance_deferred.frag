/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

// Vertex Stage: post_processor.vert

#version 450

#include "light_inputs_decoder.glsl"
#include "post_processor.glsl"
#include "radiance_point_light.glsl"
#include "radiance_spot_light.glsl"
#include "radiance_sun_light.glsl"
#include "occlusion_spot_light.glsl"

struct SunLight {
    vec4 direction;
    vec4 intensity;
};

struct PointLight {
    vec4 position;
    vec4 intensity;
};

struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 intensity;
    float cos_outer_cone;
    float cone_normalizer;
};

struct ShadowedSpotLight {
    mat4 to_light_space;
    vec4 position;
    vec4 direction;
    vec4 intensity;
    float cos_outer_cone;
    float cone_normalizer;
};

layout (push_constant) uniform RadianceMapperConstants {
    float rec_x_a;
    float rec_y_a;
    float rec_z_a;
    float rec_z_b;
} rmc;

const int kCascadedShadowMapLayers = 4;
const int kMaxShadowedSpotLights = 16;
const int kMaxSpotLights = 64;
const int kMaxPointLights = 64;

layout (set = 2, binding = 0) uniform SunLightConstants {
    SunLight light_source;
} sun_light;

layout (set = 2, binding = 1) uniform sampler2D sun_light_shadow_maps[kCascadedShadowMapLayers];

layout (set = 2, binding = 2) uniform ShadowedSpotLightConstants {
    int num_lights;
    ShadowedSpotLight light_sources[kMaxShadowedSpotLights];
} shadowed_spot_lights;

layout (set = 2, binding = 3) uniform sampler2D spotlight_shadow_map[kMaxShadowedSpotLights];

layout (set = 2, binding = 4) uniform SpotLightConstants {
    int num_lights;
    SpotLight light_sources[kMaxSpotLights];
} spot_lights;

layout (set = 2, binding = 5) uniform PointLightConstants {
    int num_lights;
    PointLight light_sources[kMaxPointLights];
} point_lights;

layout (location = 0) out vec4 out_radiance;

void main()
{
    vec2 screen_tex_coord = ScreenTexCoord();

    vec3 albedo = DecodeAlbedo(screen_tex_coord);
    vec3 normal = DecodeNormal(screen_tex_coord);
    float roughness = DecodeRoughness(screen_tex_coord);
    float metallic = DecodeMetallic(screen_tex_coord);

    vec3 hit_point = DecodePosition(screen_tex_coord,
                                    rmc.rec_x_a, rmc.rec_y_a,
                                    rmc.rec_z_a, rmc.rec_z_b);

    vec3 result;

    vec3 sun_radiance = SunLightRadiance(
        vec3(sun_light.light_source.direction),
        vec3(sun_light.light_source.intensity),
        normal,
        albedo,
        roughness,
        metallic);
    result += sun_radiance;

    for (int i = 0; i < shadowed_spot_lights.num_lights; i++) {
        vec3 radiance = SpotLightRadiance(
            vec3(shadowed_spot_lights.light_sources[i].direction),
            vec3(shadowed_spot_lights.light_sources[i].position),
            shadowed_spot_lights.light_sources[i].cos_outer_cone,
            shadowed_spot_lights.light_sources[i].cone_normalizer,
            vec3(shadowed_spot_lights.light_sources[i].intensity),
            hit_point,
            normal,
            albedo,
            roughness,
            metallic);
        float occlusion = SpotLightOcclusion(
            hit_point,
            shadowed_spot_lights.light_sources[i].to_light_space,
            spotlight_shadow_map[i]);
        result += occlusion*radiance;
    }

    for (int i = 0; i < spot_lights.num_lights; i++) {
        vec3 radiance = SpotLightRadiance(
            vec3(spot_lights.light_sources[i].position),
            vec3(spot_lights.light_sources[i].direction),
            spot_lights.light_sources[i].cos_outer_cone,
            spot_lights.light_sources[i].cone_normalizer,
            vec3(spot_lights.light_sources[i].intensity),
            hit_point,
            normal,
            albedo,
            roughness,
            metallic);
        result += radiance;
    }

    for (int i = 0; i < point_lights.num_lights; ++i) {
        vec3 radiance = PointLightRadiance(
                    vec3(point_lights.light_sources[i].position),
                    vec3(point_lights.light_sources[i].intensity),
                    hit_point,
                    normal,
                    albedo,
                    roughness,
                    metallic);
        result += radiance;
    }

    out_radiance = vec4(result, 1.0);
}
