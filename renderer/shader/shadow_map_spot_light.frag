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
    float rec_z_a;
    float rec_z_b;
} ppc;

layout(set = 1, binding = 0) uniform sampler2D depth_map;

layout (location = 0) out float out_occluder_depth;

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();
    float ndc_depth = texture(depth_map, screen_tex_coord).x;
    out_occluder_depth = ppc.rec_z_a/(ppc.rec_z_b - ndc_depth);
}
