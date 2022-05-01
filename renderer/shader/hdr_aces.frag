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

#include "post_processor.glsl"

layout(set = 1, binding = 0) uniform sampler2D radiance_map;
layout(set = 1, binding = 1) uniform sampler2D exposure_pixel;

layout (location = 0) out vec4 out_radiance;

vec3 AcesToneMapping(vec3 radiance, float exposure) {
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;

    vec3 color = radiance * exposure;
    vec3 ldr = color * (color * A + B) / (color * (color * C + D) + E);

    return clamp(ldr, 0.0f, 1.0f);
}

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    vec3 radiance = texture(radiance_map, screen_tex_coord).xyz;
    float log_exposure = texture(exposure_pixel, vec2(0.0, 0.0f)).x;
    float exposure = exp(log_exposure);
    vec3 color = AcesToneMapping(radiance, exposure);
    float luminance = sqrt(dot(color, vec3(0.299, 0.587, 0.114)));

    out_radiance = vec4(color, luminance);
}
