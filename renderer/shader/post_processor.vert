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

void main() {
    const vec2 vertices[4] = vec2[4](
		vec2(-1.0f, -1.0f),
		vec2( 1.0f, -1.0f),
		vec2( 1.0f,  1.0f),
        vec2(-1.0f,  1.0f)
	);

    const uint indices[6] = uint[6](
        0, 2, 1, 0, 3, 2
    );

    uint index = indices[gl_VertexIndex + gl_InstanceIndex*3];
    gl_Position = vec4(vertices[index], 1.0f, 1.0f);
}
