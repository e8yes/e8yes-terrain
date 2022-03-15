#version 450

layout (push_constant) uniform PerPassConstants {
	float z_near;
    float z_far;
    float alpha;
} ppc;

layout(set = 1, binding = 0) uniform sampler2D depth_map;

layout (location = 0) in vec2 in_texcoord;
layout (location = 0) out vec4 out_frag_color;

float RecoverTrueDepth(float ndc_depth, float z_near, float z_far) {
    // ndc_depth = a*1/z + b, where a = -z_near*z_far/(z_far - z_near)
    // and b = -z_near/(z_far - z_near).
    // => z = a/(ndc_depth - b), depth = abs(z)
    // => depth = -a/(ndc_depth - b)
    float a = -z_near*z_far/(z_far - z_near);
    float b = -z_near/(z_far - z_near);

    return -a/(ndc_depth - b);
}

float NormalizeDepth(float depth, float z_near, float z_far) {
    float normalized_value = (z_far - depth) / (z_far - z_near);
    return clamp(normalized_value, 0.0f, 1.0f);
}

void main() {
    float ndc_depth = texture(depth_map, in_texcoord).x;
    float true_depth = RecoverTrueDepth(ndc_depth, ppc.z_near, ppc.z_far);
    float normalized_depth = NormalizeDepth(true_depth, ppc.z_near, ppc.z_far);

    float final_value = (1.0f - ppc.alpha)*ndc_depth + ppc.alpha*normalized_depth;

    out_frag_color = vec4(final_value, final_value, final_value, 1.0);
}
