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

#include "brdf.glsl"

vec4 Radiance(vec3 incident_intensity,
             vec3 incident_ray,
             vec3 normal,
             vec3 albedo,
             float metallic,
             float roughness) {
    vec3 exitent_ray = vec3(0.0f, 0.0f, 1.0f);
    vec3 half_way = normalize(incident_ray + exitent_ray);

    float cos_h_o = max(0.0f, dot(half_way, exitent_ray));
    float cos_n_i = max(0.0f, dot(normal, incident_ray));
    float cos_n_o = max(0.0f, dot(normal, exitent_ray));
    float cos_n_h = max(0.0f, dot(normal, half_way));

    vec3 diffuse_f0 = albedo * (1.0f - metallic);
    vec3 specular_f0 = mix(vec3(0.04f), albedo, metallic);

    vec3 diffuse_brdf = LambertianDiffuseBrdf(diffuse_f0);
    vec3 specular_brdf = GgxSpecularBrdf(specular_f0, roughness,
                                      cos_h_o, cos_n_i, cos_n_o, cos_n_h);
    vec3 final_brdf = diffuse_brdf + specular_brdf;

    vec3 radiance = incident_intensity * final_brdf * cos_n_i;

    return vec4(radiance, 1.0f);
}
