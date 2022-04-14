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

vec3 Fresnel(vec3 f0, float cos_h_o)
{
    return f0 + (1.0f - f0) * pow(clamp(1.0f - cos_h_o, 0.0f, 1.0f), 5.0f);
}

float GgxVisibility(float NdotL, float NdotV, float slope_stddev)
{
    float slope_var = slope_stddev * slope_stddev;

    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0f - slope_var) + slope_var);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0f - slope_var) + slope_var);

    float GGX = GGXV + GGXL;
    if (GGX > 0.0f)
    {
        return 0.5f / GGX;
    }
    return 0.0f;
}

float GgxDistribution(float cos_n_h, float slope_stddev)
{
    float slope_var = slope_stddev * slope_stddev;
    float f = (cos_n_h * cos_n_h) * (slope_var - 1.0f) + 1.0f;
    return slope_var / (M_PI * f * f);
}

vec3 LambertianDiffuseBrdf(vec3 albedo)
{
    return albedo / M_PI;
}

vec3 GgxSpecularBrdf(vec3 f0,
                   float slope_stddev,
                   float cos_h_o,
                   float cos_n_i,
                   float cos_n_o,
                   float cos_n_h)
{
    vec3 fresnel = Fresnel(f0, cos_h_o);
    float ggx_visbility = GgxVisibility(cos_n_i, cos_n_o, slope_stddev);
    float ggx_distribution = GgxDistribution(cos_n_h, slope_stddev);

    return fresnel * ggx_visbility * ggx_distribution;
}

vec3 FresnelMixBrdf(vec3 f0_color, float ior, float cos_h_o, float metallic_factor,
                  vec3 base_brdf, vec3 layer_brdf) {
    vec3 fr = Fresnel(f0_color, cos_h_o);
    return mix(base_brdf, layer_brdf, fr);
}
