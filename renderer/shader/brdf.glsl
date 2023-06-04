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

#ifndef BRDF_GLSL
#define BRDF_GLSL

const float M_PI = 3.1415926535897932384626433832795;
const float INV_M_PI = 1.0f/M_PI;

vec3 SchlickFresnel(vec3 r0, float cos_h_i) {
    float compl_cos_h_i = max(0.0f, 1.0f - cos_h_i);
    float compl_cos_h_i2 = compl_cos_h_i*compl_cos_h_i;
    float compl_cos_h_i4 = compl_cos_h_i2*compl_cos_h_i2;
    float compl_cos_h_i5 = compl_cos_h_i4*compl_cos_h_i;
    return mix(r0, vec3(1.0f), compl_cos_h_i5);
}

float GgxInvGeometry(float cos_n_i, float cos_n_o, float alpha) {
    float smooth_surface = 2.0f*cos_n_i*cos_n_o;
    float rough_surface = cos_n_i + cos_n_o;
    return 2.0f*mix(smooth_surface, rough_surface, alpha);
}

float GgxDistribution(float cos_n_h, float alpha2) {
    float cos_n_h2 = cos_n_h*cos_n_h;
    float c = cos_n_h2*(alpha2 - 1.0f) + 1.0f;
    return alpha2/(M_PI * c*c);
}

vec3 RoughDiffuseBrdf(vec3 albedo,
                      float alpha,
                      float cos_i_o,
                      float cos_n_i,
                      float cos_n_o,
                      float cos_n_h)
{
    float smooth_surface = 1.0f;

    float facing = 0.5f + 0.5f*cos_i_o;
    float rough_surface = facing*(0.9f - 0.4f*facing)*cos_n_h/(cos_n_h + 0.5f);

    float single = INV_M_PI*mix(smooth_surface, rough_surface, alpha);
    float multi = 0.1159f*alpha;

    return albedo*(single + albedo*multi);
}

float GgxSpecularBrdf(float alpha,
                    float cos_n_i,
                    float cos_n_o,
                    float cos_n_h)
{
    float d = GgxDistribution(cos_n_h, alpha*alpha);
    float inv_g = GgxInvGeometry(cos_n_i, cos_n_o, alpha);
    return d/inv_g;
}

vec3 FresnelMix(vec3 r0, float cos_h_i, vec3 diffuse, float specular) {
    vec3 f = SchlickFresnel(r0, cos_h_i);
    return mix(diffuse, vec3(specular), f);
}

#endif // BRDF_GLSL
