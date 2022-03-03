#version 450

layout (location = 0) out vec2 out_texcoord;

void main() {
    const vec2 vertices[4] = vec2[4](
		vec2(-1.0f, -1.0f),
		vec2( 1.0f, -1.0f),
		vec2( 1.0f,  1.0f),
        vec2(-1.0f,  1.0f)
	);

    const vec2 texcoords[4] = vec2[4](
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f)
	);

    const uint indices[6] = uint[6](
        0, 1, 3, 1, 2, 3
    );

    uint index = indices[gl_VertexIndex + gl_InstanceIndex*3];

    out_texcoord = texcoords[index];
    gl_Position = vec4(vertices[index], 1.0f, 1.0f);
}
