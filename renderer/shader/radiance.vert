#version 450

layout (push_constant) uniform PerObjectConstants {
	mat4 proj_view_model;
} poc;

layout (location = 0) in vec3 in_position;
layout (location = 3) in vec2 in_tex_coord;
layout (location = 0) out vec2 out_tex_coord;

void main() {
    out_tex_coord = in_tex_coord;
    gl_Position = poc.proj_view_model * vec4(in_position, 1.0f);
}
