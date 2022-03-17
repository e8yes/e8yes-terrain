#version 450

void main() {
    const vec2 vertices[4] = vec2[4](
		vec2(-1.0f, -1.0f),
		vec2( 1.0f, -1.0f),
		vec2( 1.0f,  1.0f),
        vec2(-1.0f,  1.0f)
	);

    const uint indices[6] = uint[6](
        0, 2, 1, 0, 3, 2
    );

    uint index = indices[gl_VertexIndex + gl_InstanceIndex*3];
    gl_Position = vec4(vertices[index], 1.0f, 1.0f);
}
