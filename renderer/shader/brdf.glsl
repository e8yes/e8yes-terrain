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


const float M_PI = 3.1415926535897932384626433832795f;
const float INV_M_PI = 1.0f/3.1415926535897932384626433832795f;

float Fresnel(float cos_h_o) {
    float compl_cos_h_o = max(0.0f, 1.0f - cos_h_o);
    float a = 4.0f*pow(compl_cos_h_o, 5.0f) + 3.0f;
    float b = 4.0f + 1.5f*1.5f;
    return a/b;
}

float GgxLambda(float cos_n_w, float slope_var) {
    float cos_n_w2 = cos_n_w*cos_n_w;
    float sin_n_w2 = max(0.0f, 1.0f - cos_n_w2);
    float tan_n_w2 = sin_n_w2 / cos_n_w2;
    return 2.0f/(1.0f + sqrt(1.0f + slope_var*tan_n_w2));
}

float GgxGeometry(float cos_n_i, float cos_n_o, float slope_var) {
    return GgxLambda(cos_n_i, slope_var)*GgxLambda(cos_n_o, slope_var);
}

float GgxDistribution(float cos_n_h, float slope_var) {
    float cos_n_h2 = cos_n_h*cos_n_h;
    float cos_n_h4 = cos_n_h2*cos_n_h2;

    float sin_n_h2 = max(0.0f, 1.0f - cos_n_h2);
    float tan_n_h2 = sin_n_h2/cos_n_h2;

    float c = slope_var + tan_n_h2;
    float c2 = c*c;

    return slope_var/(M_PI * cos_n_h4 * c2);
}

vec3 FresnelDiffuseBrdf(vec3 albedo,
                        float slope_stddev,
                        float cos_h_o,
                        float cos_n_i,
                        float cos_n_o)
{
    float fd90 = 0.5f + 2.0f*slope_stddev*cos_h_o*cos_h_o;

    float compl_cos_n_i = max(0.0f, 1.0f - cos_n_i);
    float compl_cos_n_o = max(0.0f, 1.0f - cos_n_o);

    float fresnel = (1.0f + (fd90 - 1.0f)*pow(compl_cos_n_i, 5.0f))*
            (1.0f + (fd90 - 1.0f)*pow(compl_cos_n_o, 5.0f));

    return fresnel*albedo*INV_M_PI;
}

vec3 GgxSpecularBrdf(vec3 f0,
                   float slope_stddev,
                   float cos_h_o,
                   float cos_n_i,
                   float cos_n_o,
                   float cos_n_h)
{
    vec3 f = f0*Fresnel(cos_h_o);

    float slope_var = slope_stddev*slope_stddev;
    float d = GgxDistribution(cos_n_h, slope_var);
    float g = GgxGeometry(cos_n_i, cos_n_o, slope_var);

    float normalizer = 4.0f*cos_n_i*cos_n_o + 1e-2f;

    return clamp(f*d*g/normalizer, 0.0f, 2.0f);
}

vec3 FresnelMixBrdf(vec3 f0_color, float ior, float cos_h_o, float metallic_factor,
                  vec3 base_brdf, vec3 layer_brdf) {
    vec3 fr = f0_color*Fresnel(cos_h_o);
    return mix(base_brdf, layer_brdf, fr);
}
