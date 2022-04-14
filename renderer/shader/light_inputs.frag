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

// Vertex Stage: light_inputs.vert

#version 450

#include "light_inputs_encoder.glsl"

layout(set = 2, binding = 0) uniform sampler2D albedo_map;
layout(set = 2, binding = 1) uniform sampler2D normal_map;
layout(set = 2, binding = 2) uniform sampler2D roughness_map;
layout(set = 2, binding = 3) uniform sampler2D metallic_map;

layout (location = 0) in vec3 in_view_normal;
layout (location = 1) in vec3 in_view_tangent;
layout (location = 2) in vec2 in_tex_coord;


vec3 ComputeNormal(vec3 n, vec3 t, sampler2D normal_map, vec2 tex_coord) {
	vec3 b = normalize(cross(n, t));

	mat3 to_tangent_space = mat3(t, b, n);
	mat3 to_view_space = transpose(to_tangent_space);

	vec3 tangent_space_normal = texture(normal_map, tex_coord).xyz;
	return to_view_space*tangent_space_normal;
}

void main() {
    vec3 albedo = texture(albedo_map, in_tex_coord).xyz;
    // vec3 n = normalize(in_view_normal);
    // vec3 t = normalize(in_view_tangent);
    // vec3 normal = ComputeNormal(n, t, normal_map, in_tex_coord);
    vec3 normal = normalize(in_view_normal);
    float roughness = texture(roughness_map, in_tex_coord).x;
    float metallic = texture(metallic_map, in_tex_coord).x;

    Encode(albedo, normal, roughness, metallic);
}
