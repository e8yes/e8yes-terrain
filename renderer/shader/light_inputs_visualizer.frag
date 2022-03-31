// Vertex Stage: post_processor.vert

#version 450

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float inv_viewport_width;
    float inv_viewport_height;
} pfc;

layout (push_constant) uniform PerPassConstants {
	int input_to_visualize;
} ppc;

layout(set = 1, binding = 0) uniform sampler2D geometry_map;

layout (location = 0) out vec4 out_frag_color;

vec2 ScreenCoord() {
    return vec2(gl_FragCoord.x / pfc.inv_viewport_width, 
                gl_FragCoord.y / pfc.inv_viewport_height);
}

void main() {
    vec2 screen_tex_coord = ScreenCoord();

    if (ppc.input_to_visualize == 1) {
        // Visualizes the normal vector.
        vec3 normal = texture(geometry_map, screen_tex_coord).xyz;
        out_frag_color = vec4(normal, 1.0);
    } else {
        // Visualizes the roughness factor.
        float roughness = texture(geometry_map, screen_tex_coord).w;
        out_frag_color = vec4(roughness, roughness, roughness, 1.0);
    }
}

