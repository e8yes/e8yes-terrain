#version 450

layout (location = 0) in vec2 in_texcoord;

layout (location = 0) out vec4 out_frag_color;

void main() {
    out_frag_color = vec4(
        in_texcoord.x,
        (in_texcoord.x + in_texcoord.y)/2,
        in_texcoord.y,
        1.0);
}
