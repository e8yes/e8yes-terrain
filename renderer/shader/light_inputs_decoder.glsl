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

layout(set = 1, binding = 0) uniform sampler2D normal_roughness;
layout(set = 1, binding = 1) uniform sampler2D albedo_metallic;

vec3 DecodeAlbedo(vec2 screen_tex_coord) {
    return texture(albedo_metallic, screen_tex_coord).xyz;
}

vec3 DecodeRawNormal(vec2 screen_tex_coord) {
    return texture(normal_roughness, screen_tex_coord).xyz;
}

vec3 DecodeNormal(vec2 screen_tex_coord) {
    vec3 encoded_normal = DecodeRawNormal(screen_tex_coord);
    return 2.0f * encoded_normal - 1.0f;
}

float DecodeRoughness(vec2 screen_tex_coord) {
    return texture(normal_roughness, screen_tex_coord).w;
}

float DecodeMetallic(vec2 screen_tex_coord) {
    return texture(albedo_metallic, screen_tex_coord).w;
}
