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
    float z_near;
    float z_far;
} ppc;

layout(set = 1, binding = 0) uniform sampler2D ndc_depth_map;

layout (location = 0) out float linear_depth;

float ToLinearDepth(float ndc_depth, float z_near, float z_far) {
    // ndc_depth = a*1/z + b, where a = -z_near*z_far/(z_far - z_near)
    // and b = -z_near/(z_far - z_near).
    // => z = a/(ndc_depth - b), depth = abs(z)
    // => depth = -a/(ndc_depth - b)
    float a = -z_near*z_far/(z_far - z_near);
    float b = -z_near/(z_far - z_near);

    return -a/(ndc_depth - b);
}

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    float ndc_depth = texture(ndc_depth_map, screen_tex_coord).x;
    linear_depth = ToLinearDepth(ndc_depth, ppc.z_near, ppc.z_far);
}
