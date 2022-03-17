#version 450

layout (push_constant) uniform PerObjectConstants {
	mat4 view_model;
	mat4 proj_view_model;
} poc;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_tex_coord;

layout (location = 0) out vec3 out_view_normal;
layout (location = 1) out vec3 out_view_tangent;
layout (location = 2) out vec2 out_tex_coord;

void main() {
	out_view_normal = vec3(poc.view_model * vec4(in_normal, 0.0f));
	out_view_tangent = vec3(poc.view_model * vec4(in_tangent, 0.0f));
	out_tex_coord = in_tex_coord;

	gl_Position = poc.proj_view_model * vec4(in_position, 1.0f);
}
