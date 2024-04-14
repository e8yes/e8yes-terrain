/**
 * e8yes islands
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

#version 450

layout (push_constant) uniform ObjectTransform {
	mat4 view_model;
	mat4 proj_view_model;
} push_constants;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coord;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec2 out_tex_coord;

void main() {
    vec4 view_position = push_constants.view_model * vec4(in_position, 1.0f);
    out_position = vec3(view_position);
    out_tex_coord = in_tex_coord;

    gl_Position = push_constants.proj_view_model * vec4(in_position, 1.0f);
}
