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
#include "radiance.glsl"

layout (push_constant) uniform PerLightConstants {
    vec4 dir;
    vec4 intensity;
} plc;

layout (location = 0) out vec4 out_radiance;

void main() {
    vec2 screen_tex_coord = ScreenTexCoord();

    vec3 albedo = DecodeAlbedo(screen_tex_coord);
    vec3 normal = DecodeNormal(screen_tex_coord);
    float roughness = DecodeRoughness(screen_tex_coord);
    float metallic = DecodeMetallic(screen_tex_coord);

    vec3 incident_intensity = vec3(plc.intensity);
    vec3 incident_ray = -vec3(plc.dir);
    out_radiance = Radiance(incident_intensity, incident_ray, normal,
                         albedo, metallic, roughness);
}
