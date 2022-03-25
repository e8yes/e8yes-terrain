// Vertex Stage: radiance.vert

#version 450

#define M_PI 3.1415926535897932384626433832795f

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

layout(set = 2, binding = 0) uniform sampler2D albedo_map;
layout(set = 2, binding = 1) uniform sampler2D metallic_map;
layout(set = 2, binding = 2) uniform sampler2D indirect_radiosity_map;

layout(set = 2, binding = 3) uniform sampler2D unshoot_diffuse_radiance_map;
layout(set = 2, binding = 4) uniform sampler2D unshoot_specular_radiance_map;

layout (location = 0) in vec2 in_tex_coord;
layout (location = 0) out vec4 out_radiance;

void main() {
    vec2 screen_tex_coord = vec2(
        gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);

    vec3 albedo = texture(albedo_map, in_tex_coord).xyz;
    float metallic = texture(metallic_map, in_tex_coord).x;
    vec3 indirect_radiosity = texture(indirect_radiosity_map, in_tex_coord).xyz;
    vec3 unshoot_diffuse_radiance = texture(
        unshoot_diffuse_radiance_map, screen_tex_coord).xyz;
    vec3 unshoot_specular_radiance = texture(
        unshoot_specular_radiance_map, screen_tex_coord).xyz;

    vec3 unshoot_radiance = (1.0f - metallic)*unshoot_diffuse_radiance +
        metallic*unshoot_specular_radiance;
    vec3 radiance = albedo*unshoot_radiance + indirect_radiosity/M_PI;

    out_radiance = vec4(radiance, 1.0f);
}
