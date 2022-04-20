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

layout (location = 0) out vec4 out_normal_roughness;
layout (location = 1) out vec4 out_albedo_metallic;

void Encode(vec3 albedo, vec3 normal, float roughness, float metallic) {
    out_normal_roughness = vec4(normal.x, normal.y, normal.z, roughness);
    out_albedo_metallic = vec4(albedo.x, albedo.y, albedo.z, metallic);
}
