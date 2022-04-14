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

#version 450

layout (push_constant) uniform PerObjectConstants {
	mat4 view_model;
	mat4 proj_view_model;
} poc;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_tex_coord;

layout (location = 0) out vec3 out_view_normal;
layout (location = 1) out vec3 out_view_tangent;
layout (location = 2) out vec2 out_tex_coord;

void main() {
    out_view_normal = vec3(poc.view_model * vec4(in_normal, 0.0f));
    out_view_tangent = vec3(poc.view_model * vec4(in_tangent, 0.0f));
    out_tex_coord = in_tex_coord;

	gl_Position = poc.proj_view_model * vec4(in_position, 1.0f);
}
