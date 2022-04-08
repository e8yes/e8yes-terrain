// Vertex Stage: post_processor.vert

#version 450

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

layout(set = 2, binding = 0) uniform sampler2D radiance_map;
layout(set = 2, binding = 1) uniform sampler2D exposure_pixel;

layout (location = 0) out vec4 out_radiance;

vec3 AcesToneMapping(vec3 radiance, float exposure) {
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;

    vec3 color = radiance * exposure;
    vec3 ldr = color * (color * A + B) / (color * (color * C + D) + E);

    return clamp(ldr, 0.0f, 1.0f);
}

void main() {
    vec2 screen_tex_coord = vec2(
        gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);

    vec3 radiance = texture(radiance_map, screen_tex_coord).xyz;
    float exposure = texture(exposure_pixel, vec2(0.0, 0.0f)).x;
    vec3 mapped = AcesToneMapping(radiance, exposure);

    out_radiance = vec4(mapped, 1.0f);
}
