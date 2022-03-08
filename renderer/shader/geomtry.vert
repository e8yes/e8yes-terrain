#version 450

layout (set = 0, binding = 0) uniform RenderPassConstants {
	mat4 proj_view;
} rpc;

layout (push_constant) uniform PerObjectConstants {
	mat4 model;
} poc;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_texcoord;

layout (location = 0) out vec3 out_world_position;
layout (location = 1) out vec3 out_world_normal;
layout (location = 2) out vec3 out_world_tangent;
layout (location = 3) out vec2 out_texcoord;

void main() {
	vec4 world_position = poc.model * vec4(in_position, 1.0f);

	out_world_position = vec3(world_position);
	out_world_normal = vec3(poc.model * vec4(in_normal, 0.0f));
	out_world_tangent = vec3(poc.model * vec4(in_tangent, 0.0f));
	out_texcoord = in_texcoord;

	gl_Position = rpc.proj_view * world_position;
}
