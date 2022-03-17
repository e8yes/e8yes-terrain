// Vertex Stage: post_processor.vert

#version 450

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

layout (push_constant) uniform PerPassConstants {
	float z_near;
    float z_far;
} ppc;

layout(set = 1, binding = 0) uniform sampler2D depth_map;
layout (location = 0) out float out_z;

float RecoverZValue(float ndc_depth, float z_near, float z_far) {
    // ndc_depth = a*1/z + b, where a = -z_near*z_far/(z_far - z_near)
    // and b = -z_near/(z_far - z_near).
    // => z = a/(ndc_depth - b)
    float a = -z_near*z_far/(z_far - z_near);
    float b = -z_near/(z_far - z_near);

    return a/(ndc_depth - b);
}

void main() {
    vec2 screen_tex_coord = vec2(
        gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);

    float ndc_depth = texture(depth_map, screen_tex_coord).x;
    out_z = RecoverZValue(ndc_depth, ppc.z_near, ppc.z_far);
}
