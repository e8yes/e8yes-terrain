#version 450

layout (set = 0, binding = 0) uniform ProjectionData {
	mat4 trans;
} projection;

layout (push_constant) uniform Constants {
	mat4 model_view_trans;
	mat4 normal_trans;
} push_consts;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

layout (location = 0) out vec3 out_cam_position;
layout (location = 1) out vec3 out_cam_normal;
layout (location = 2) out vec2 out_texcoord;

void main() {
	vec4 homo_cam_position = push_consts.model_view_trans * vec4(in_position, 1.0f);
	out_cam_position = vec3(homo_cam_position);

	out_cam_normal = vec3(push_consts.normal_trans * vec4(in_normal, 0.0f));

	gl_Position = projection.trans * homo_cam_position;
}