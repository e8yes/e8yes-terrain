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

#ifndef RADIANCE_POINT_LIGHT_GLSL
#define RADIANCE_POINT_LIGHT_GLSL

#include "radiance.glsl"

vec3 PointLightRadiance(vec3 position,
                        vec3 intensity,
                        vec3 hit_point,
                        vec3 normal,
                        vec3 albedo,
                        float roughness,
                        float metallic) {
    // Incident ray and inverse square attenuation.
    vec3 light_ray = position - hit_point;
    float inv_r2 = 1.0f/dot(light_ray, light_ray);
    vec3 incident_intensity = intensity*inv_r2;
    vec3 incident_ray = light_ray*sqrt(inv_r2);

    // Standard radiance calculation.
    return Radiance(incident_intensity, incident_ray, normal,
                    albedo, metallic, roughness);
}

#endif // RADIANCE_POINT_LIGHT_GLSL
