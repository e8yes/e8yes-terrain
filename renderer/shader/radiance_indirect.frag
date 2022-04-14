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

// Vertex Stage: radiance.vert

#version 450

#include "post_processor.glsl"

layout(set = 2, binding = 0) uniform sampler2D direct_radiance_map;
layout(set = 2, binding = 1) uniform sampler2D indirect_radiance_map;

layout (location = 0) out vec4 out_radiance;

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    vec3 direct_radiance = texture(direct_radiance_map, screen_tex_coord).xyz;
    vec3 indirect_radiance = texture(indirect_radiance_map, screen_tex_coord).xyz;

    vec3 final_radiance = direct_radiance + indirect_radiance;
    out_radiance = vec4(final_radiance, 1.0f);
}
