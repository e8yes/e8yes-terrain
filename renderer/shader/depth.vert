#version 450

layout (push_constant) uniform PerObjectConstants {
	mat4 proj_view_model;
} poc;

layout (location = 0) in vec3 in_position;

void main() {
	gl_Position = poc.proj_view_model * vec4(in_position, 1.0f);
}