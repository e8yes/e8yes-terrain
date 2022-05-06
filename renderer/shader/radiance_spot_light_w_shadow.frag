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

// Vertex Stage: post_processor.vert

#version 450

#include "light_inputs_decoder.glsl"
#include "post_processor.glsl"
#include "radiance_spot_light.glsl"
#include "shadow_map.glsl"

layout (push_constant) uniform PerLightConstants {
    mat4 to_light_space;

    vec4 position;
    vec4 direction;
    vec4 intensity;
    float cos_outer_cone;
    float cone_normalizer;

    float rec_x_a;
    float rec_y_a;
    float rec_z_a;
    float rec_z_b;
} plc;

layout(set = 1, binding = 3) uniform sampler2D shadow_map;

layout (location = 0) out vec4 out_radiance;

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    vec3 albedo = DecodeAlbedo(screen_tex_coord);
    vec3 normal = DecodeNormal(screen_tex_coord);
    float roughness = DecodeRoughness(screen_tex_coord);
    float metallic = DecodeMetallic(screen_tex_coord);

    vec3 hit_point = DecodePosition(screen_tex_coord,
                                    plc.rec_x_a, plc.rec_y_a,
                                    plc.rec_z_a, plc.rec_z_b);

    vec3 radiance = SpotLightRadiance(vec3(plc.position), vec3(plc.direction),
                                     plc.cos_outer_cone,
                                     plc.cone_normalizer,
                                     vec3(plc.intensity),
                                     hit_point, normal,
                                     albedo, roughness, metallic);
    float occlusion = SpotLightOcclusion(hit_point, plc.to_light_space, shadow_map);
    radiance *= occlusion;

    out_radiance = vec4(radiance, 1.0);
}
