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

#ifndef LIGHT_INPUTS_DECODER_GLSL
#define LIGHT_INPUTS_DECODER_GLSL

layout(set = 1, binding = 0) uniform sampler2D normal_roughness;
layout(set = 1, binding = 1) uniform sampler2D albedo_metallic;
layout(set = 1, binding = 2) uniform sampler2D depth;

float ToViewSpaceZ(float ndc_depth, float a, float b) {
    // ndc_depth = a*1/z + b, where a = -z_near*z_far/(z_far - z_near)
    // and b = -z_near/(z_far - z_near).
    // => z = a/(ndc_depth - b)
    return a/(ndc_depth - b);
}

float ToViewSpaceX(float screen_space_x, float z, float a) {
    // ndc_x = 2*z_near/sensor_width*x*(-1/z), where ndc_x = 2*screen_space_x - 1.
    // => x = -sensor_width/(2*z_near)*ndc_x*z
    // Let a = -sensor_width/(2*z_near)
    // => x = a*ndc_x*z
    float ndc_x = 2.0f*screen_space_x - 1.0f;
    return a*ndc_x*z;
}

float ToViewSpaceY(float screen_space_y, float z, float a) {
    // ndc_y = 2*z_near/sensor_height*y*(-1/z), ndc_y = 2*screen_space_y - 1.
    // => y = -sensor_height/(2*z_near)*ndc_y*z
    // Let a = -sensor_height/(2*z_near)
    // => y = a*ndc_y*z
    float ndc_y = 2.0f*screen_space_y - 1.0f;
    return a*ndc_y*z;
}

vec3 DecodePosition(vec2 screen_tex_coord,
                    float rec_x_a,
                    float rec_y_a,
                    float rec_z_a,
                    float rec_z_b) {
    float ndc_depth = texture(depth, screen_tex_coord).x;
    float z = ToViewSpaceZ(ndc_depth, rec_z_a, rec_z_b);
    float x = ToViewSpaceX(screen_tex_coord.x, z, rec_x_a);
    float y = ToViewSpaceY(screen_tex_coord.y, z, rec_y_a);
    return vec3(x, y, z);
}

vec3 DecodeAlbedo(vec2 screen_tex_coord) {
    return texture(albedo_metallic, screen_tex_coord).xyz;
}

vec3 DecodeNormal(vec2 screen_tex_coord) {
    return texture(normal_roughness, screen_tex_coord).xyz;
}

float DecodeRoughness(vec2 screen_tex_coord) {
    return texture(normal_roughness, screen_tex_coord).w;
}

float DecodeMetallic(vec2 screen_tex_coord) {
    return texture(albedo_metallic, screen_tex_coord).w;
}

#endif // LIGHT_INPUTS_DECODER_GLSL
