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

#ifndef RADIANCE_GLSL
#define RADIANCE_GLSL

#include "brdf.glsl"

const vec3 DIELETRIC_R0 = vec3(0.04f);

vec3 Radiance(vec3 incident_intensity,
             vec3 incident_ray,
             vec3 normal,
             vec3 albedo,
             float metallic,
             float roughness) {
    vec3 exitent_ray = vec3(0.0f, 0.0f, 1.0f);
    vec3 half_way = normalize(incident_ray + exitent_ray);

    float cos_h_i = max(0.0f, dot(half_way, incident_ray));
    float cos_n_i = max(0.0f, dot(normal, incident_ray));
    float cos_n_o = max(0.0f, dot(normal, exitent_ray));
    float cos_i_o = max(0.0f, dot(incident_ray, exitent_ray));
    float cos_n_h = max(0.0f, dot(normal, half_way));

    vec3 diffuse_albedo = albedo * (1.0f - metallic);
    vec3 r0 = mix(DIELETRIC_R0, albedo, metallic);
    float alpha = roughness*roughness;

    vec3 diffuse_brdf = RoughDiffuseBrdf(diffuse_albedo, alpha,
                                         cos_i_o, cos_n_i, cos_n_o, cos_n_h);
    float specular_brdf = GgxSpecularBrdf(alpha, cos_n_i, cos_n_o, cos_n_h);
    vec3 final_brdf = FresnelMix(r0, cos_h_i, diffuse_brdf, specular_brdf);

    return incident_intensity * final_brdf * cos_n_i;
}

#endif // RADIANCE_GLSL
