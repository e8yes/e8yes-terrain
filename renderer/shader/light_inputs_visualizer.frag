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

layout (push_constant) uniform PerPassConstants {
	int input_to_visualize;
} ppc;

layout (location = 0) out vec4 out_frag_color;

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    if (ppc.input_to_visualize == 1) {
        // Visualizes the albedo color.
        out_frag_color = vec4(DecodeAlbedo(screen_tex_coord), 1.0);
    } else if (ppc.input_to_visualize == 2) {
        // Visualizes the normal vector.
        vec3 normal = DecodeNormal(screen_tex_coord);
        out_frag_color = vec4(0.5f*normal + 0.5f, 1.0);
    } else if (ppc.input_to_visualize == 3) {
        // Visualizes the roughness factor.
        float roughness = DecodeRoughness(screen_tex_coord);
        out_frag_color = vec4(roughness, roughness, roughness, 1.0);
    } else {
        // Visualizes the metallic factor.
        float metallic = DecodeMetallic(screen_tex_coord);
        out_frag_color = vec4(metallic, metallic, metallic, 1.0);
    }
}
