// Vertex Stage: post_processor.vert

#version 450

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

layout(set = 2, binding = 0) uniform sampler2D radiance_map;

layout (location = 0) out vec4 out_radiance;

void main() {
    vec2 screen_tex_coord = vec2(
        gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);

    vec3 radiance = texture(radiance_map, screen_tex_coord).xyz;
    vec3 mapped = clamp(radiance, 0.0f, 1.0f);

    out_radiance = vec4(mapped, 1.0f);
}
