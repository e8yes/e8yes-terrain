#version 450

layout(set = 1, binding = 0) uniform sampler2D depth_map;

layout (location = 0) in vec2 in_texcoord;

layout (location = 0) out vec4 out_frag_color;

void main() {
    float scaled_z = texture(depth_map, in_texcoord).x;
    out_frag_color = vec4(scaled_z, scaled_z, scaled_z, 1.0);
}
