// Vertex Stage: radiance.vert

#version 450

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

layout(set = 2, binding = 0) uniform sampler2D direct_radiance_map;
layout(set = 2, binding = 1) uniform sampler2D indirect_radiance_map;

layout (location = 0) out vec4 out_radiance;

void main() {
    vec2 screen_tex_coord = vec2(
        gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);

    vec3 direct_radiance = texture(direct_radiance_map, screen_tex_coord).xyz;
    vec3 indirect_radiance = texture(indirect_radiance_map, screen_tex_coord).xyz;

    vec3 final_radiance = direct_radiance + indirect_radiance;
    out_radiance = vec4(final_radiance, 1.0f);
}
