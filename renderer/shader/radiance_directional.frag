// Vertex Stage: post_processor.vert

#version 450

#define M_PI 3.1415926535897932384626433832795f

const vec2 ior = vec2(2.93f, 3.0f);

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

layout (push_constant) uniform PerLightConstants {
	vec3 dir;
	vec3 intensity;
} plc;

layout(set = 1, binding = 0) uniform sampler2D normal_roughness_map;
layout(set = 1, binding = 1) uniform sampler2D albedo_metallic_map;

layout (location = 0) out vec4 out_radiance;

float EvalDiffuseBrdf(vec3 n, vec3 i, vec3 o, float roughness) {
    float sigma2 = roughness * roughness;
    float a = 1 - 0.5f * sigma2 / (sigma2 + 0.33f);
    float b = 0.45f * sigma2 / (sigma2 + 0.09f);

    float cos_n_i = max(0.0f, dot(n, i));
    float cos_n_o = max(0.0f, dot(n, o));

    float cos_alpha = min(cos_n_i, cos_n_o);
    float cos_beta = max(cos_n_i, cos_n_o);

    float sin_alpha = sqrt(1.0f - cos_alpha * cos_alpha);
    float sin_beta = sqrt(1.0f - cos_beta * cos_beta);

    float cos_i_o = cos_alpha * cos_beta + sin_alpha * sin_beta;
    float tan_beta = sin_beta / cos_n_o;

    return a + b * max(0.0f, cos_i_o) * sin_alpha * tan_beta;
}

float Fresnel(vec2 ior, vec3 i, vec3 h) {
    float cos_th_flip = 1.0f - dot(i, h);
    float cos_th_flip2 = cos_th_flip * cos_th_flip;
    float cos_th_flip4 = cos_th_flip2 * cos_th_flip2;
    float cos_th_flip5 = cos_th_flip4 * cos_th_flip;

    float a = (ior.x - 1) * (ior.x - 1) + 4 * ior.x * cos_th_flip5 + ior.y * ior.y;
    float b = (ior.x + 1) * (ior.x + 1) + ior.y * ior.y;

    return a / b;
}

float GgxDistribution(float alpha2, vec3 n, vec3 h) {
    float cos_th = dot(n, h);
    float cos_th2 = cos_th * cos_th;
    float tan_th2 = (1.0f - cos_th2) / cos_th2;

    float c = 1.0f + tan_th2 / alpha2;

    return 1.0f / (M_PI * alpha2 * cos_th2 * cos_th2 * c * c);
}

float Lambda(float alpha2, vec3 n, vec3 w) {
    float cos_th = dot(n, w);
    float cos_th2 = cos_th * cos_th;
    float tan_th2 = (1.0f - cos_th2) / cos_th2;

    return (-1.0f + sqrt(1.0f + alpha2 * tan_th2)) / 2.0f;
}

float GgxShadow(float alpha2, vec3 i, vec3 o, vec3 n) {
    return 1.0f / (1.0f + Lambda(alpha2, n, i) + Lambda(alpha2, n, o));
}

float EvalSpecularBrdf(vec3 n, vec3 i, vec3 o, float roughness) {
    float alpha2 = 2.0f*roughness*roughness;

    float cos_n_o = max(0.0f, dot(n, o));
    float cos_n_i = max(0.0f, dot(n, i));

    vec3 h = normalize(i + o);

    float f = Fresnel(ior, i, h);
    float d = GgxDistribution(alpha2, n, h);
    float g = GgxShadow(alpha2, i, o, n);

    return f * d * g / (4.0f * cos_n_i * cos_n_o);
}

void main() {
    vec2 screen_tex_coord = vec2(
        gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);

    vec3 normal = texture(normal_roughness_map, screen_tex_coord).xyz;
    float roughness = texture(normal_roughness_map, screen_tex_coord).w;
    vec3 albedo = texture(albedo_metallic_map, screen_tex_coord).xyz;
    float metallic = texture(albedo_metallic_map, screen_tex_coord).w;

    vec3 exitent_ray = vec3(0.0f, 0.0f, 1.0f);

    float diffuse_brdf = EvalDiffuseBrdf(normal, -plc.dir, exitent_ray, roughness);
    float specular_brdf = EvalSpecularBrdf(normal, -plc.dir, exitent_ray, roughness);
    float cos_w = max(0.0f, dot(-plc.dir, exitent_ray));

    float diffuse_term = diffuse_brdf * cos_w;
    float specular_term = cos_w * specular_brdf;
    float mix_term = (1 - metallic) * diffuse_term + metallic * specular_term;

    vec3 radiance = plc.intensity * albedo * mix_term;
    out_radiance = vec4(radiance, 1.0f);
}
