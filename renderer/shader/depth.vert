#version 450

layout (push_constant) uniform Constants {
	mat4 model_view_proj_trans;
} push_consts;

layout (location = 0) in vec3 in_position;

void main() {
	gl_Position = push_consts.model_view_proj_trans *  vec4(in_position, 1.0f);
}