/**
 * e8yes islands
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

// Vertex Stage: radiance.vert

#version 450

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

const int kCascadedShadowMapLayers = 4;
const int kMaxShadowedSpotLights = 16;
const int kMaxSpotLights = 64;
const int kMaxPointLights = 64;

layout (set = 1, binding = 0) uniform RadianceMapperConstants {
    float viewport_width;
    float viewport_height;
    int num_sun_lights_shadowed;
    int num_spot_lights_shadowed;
    int num_spot_lights;
    int num_point_lights;
} radiance_mapper_constants;

layout (set = 1, binding = 1) uniform SunLightConstants {
    SunLight light_source;
} sun_light;

// layout (set = 1, binding = 2) uniform sampler2D sun_light_shadow_maps[kCascadedShadowMapLayers];

layout (set = 1, binding = 3) uniform ShadowedSpotLightConstants {
    ShadowedSpotLight light_sources[kMaxShadowedSpotLights];
} shadowed_spot_lights;

// layout (set = 1, binding = 4) uniform sampler2D spotlight_shadow_map[kMaxShadowedSpotLights];

layout (set = 1, binding = 5) uniform SpotLightConstants {
    SpotLight light_sources[kMaxSpotLights];
} spot_lights;

layout (set = 1, binding = 6) uniform PointLightConstants {
    PointLight light_sources[kMaxPointLights];
} point_lights;

layout(set = 1, binding = 7) uniform sampler2D screen_normal_map;

layout(set = 2, binding = 0) uniform sampler2D albedo_map;
layout(set = 2, binding = 1) uniform sampler2D roughness_map;
layout(set = 2, binding = 2) uniform sampler2D metallic_map;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coord;
layout (location = 0) out vec4 out_radiance;

vec2 ScreenTexCoord(float viewport_width, float viewport_height) {
    return vec2(gl_FragCoord.x / viewport_width,
                gl_FragCoord.y / viewport_height);
}

void main()
{
    vec2 screen_coord = ScreenTexCoord(
                radiance_mapper_constants.viewport_width,
                radiance_mapper_constants.viewport_height);
    vec3 normal = texture(screen_normal_map, screen_coord).xyz;
    vec3 albedo = texture(albedo_map, in_tex_coord).xyz;
    float roughness = texture(roughness_map, in_tex_coord).x;
    float metallic = texture(metallic_map, in_tex_coord).x;
    vec3 hit_point = in_position;

    vec3 result;

    if (radiance_mapper_constants.num_sun_lights_shadowed > 0) {
        vec3 sun_radiance = SunLightRadiance(
            vec3(sun_light.light_source.direction),
            vec3(sun_light.light_source.intensity),
            normal,
            albedo,
            roughness,
            metallic);
        result += sun_radiance;
    }

//    for (int i = 0; i < radiance_mapper_constants.num_spot_lights_shadowed; i++) {
//        vec3 radiance = SpotLightRadiance(
//            vec3(shadowed_spot_lights.light_sources[i].direction),
//            vec3(shadowed_spot_lights.light_sources[i].position),
//            shadowed_spot_lights.light_sources[i].cos_outer_cone,
//            shadowed_spot_lights.light_sources[i].cone_normalizer,
//            vec3(shadowed_spot_lights.light_sources[i].intensity),
//            hit_point,
//            normal,
//            albedo,
//            roughness,
//            metallic);
//        float occlusion = SpotLightOcclusion(
//            hit_point,
//            shadowed_spot_lights.light_sources[i].to_light_space,
//            spotlight_shadow_map[i]);
//        result += occlusion*radiance;
//    }

    for (int i = 0; i < radiance_mapper_constants.num_spot_lights; i++) {
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

    for (int i = 0; i < radiance_mapper_constants.num_point_lights; ++i) {
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
