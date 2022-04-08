// Vertex Stage: light_inputs.vert

#version 450

layout(set = 2, binding = 0) uniform sampler2D albedo_map;
layout(set = 2, binding = 1) uniform sampler2D normal_map;
layout(set = 2, binding = 2) uniform sampler2D roughness_map;
layout(set = 2, binding = 3) uniform sampler2D metallic_map;

layout (location = 0) in vec3 in_view_normal;
layout (location = 1) in vec3 in_view_tangent;
layout (location = 2) in vec2 in_tex_coord;
layout (location = 0) out vec4 out_normal_roughness;
layout (location = 1) out vec4 out_albedo_metallic;

vec3 ComputeNormal(vec3 n, vec3 t, sampler2D normal_map, vec2 tex_coord) {
	vec3 b = normalize(cross(n, t));

	mat3 to_tangent_space = mat3(t, b, n);
	mat3 to_view_space = transpose(to_tangent_space);

	vec3 tangent_space_normal = texture(normal_map, tex_coord).xyz;
	return to_view_space*tangent_space_normal;
}

void main() {
	vec3 n = normalize(in_view_normal);
	vec3 t = normalize(in_view_tangent);
	vec3 normal = ComputeNormal(n, t, normal_map, in_tex_coord);

	vec3 normal_color = 0.5f*(normal + 1.0f);
	float roughness = texture(roughness_map, in_tex_coord).x;

	vec3 albedo = texture(albedo_map, in_tex_coord).xyz;
	float metallic = texture(metallic_map, in_tex_coord).x;
	
        out_normal_roughness = vec4(normal_color.x, normal_color.y, normal_color.z, roughness);
        out_albedo_metallic = vec4(albedo.x, albedo.y, albedo.z, metallic);
}
