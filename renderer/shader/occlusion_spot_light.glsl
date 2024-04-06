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

#ifndef OCCLUSION_SPOT_LIGHT_GLSL
#define OCCLUSION_SPOT_LIGHT_GLSL

float SpotLightOcclusion(vec3 hit_point, mat4 to_light_space, sampler2D shadow_map) {
    vec4 light_space_position = to_light_space*vec4(hit_point, 1.0);

    vec2 query_location = light_space_position.xy/light_space_position.w;
    if (abs(query_location.x) > 1.0 ||
            abs(query_location.y) > 1.0 ||
            light_space_position.w < 0.0) {
        return 0.0;
    }

    float occluder_depth = texture(shadow_map, query_location).x;
    float query_depth = light_space_position.w;

    if (occluder_depth < query_depth) {
        return 1.0;
    } else {
        return 0.0;
    }
}

#endif // OCCLUSION_SPOT_LIGHT_GLSL
