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

layout (push_constant) uniform PerPassConstants {
    float min_value;
    float max_value;
} ppc;

layout(set = 1, binding = 0) uniform sampler2D float_map;

layout (location = 0) out vec4 out_frag_color;

float NormalizeValue(float value, float min_value, float max_value) {
    float normalized_value = (value - min_value) / (max_value - min_value);
    return clamp(normalized_value, 0.0f, 1.0f);
}

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    float value = texture(float_map, screen_tex_coord).x;
    float normalized_value = NormalizeValue(value, ppc.min_value, ppc.max_value);

    out_frag_color = vec4(normalized_value, normalized_value, normalized_value, 1.0);
}
