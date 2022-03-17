// Vertex Stage: post_processor.vert

#version 450

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float inv_viewport_width;
    float inv_viewport_height;
} pfc;

layout (location = 0) out vec4 out_frag_color;

vec2 ScreenCoord() {
    return vec2(gl_FragCoord.x / pfc.inv_viewport_width, 
                gl_FragCoord.y / pfc.inv_viewport_height);
}

void main() {
    vec2 screen_tex_coord = ScreenCoord();

    out_frag_color = vec4(
        screen_tex_coord.x,
        (screen_tex_coord.x + screen_tex_coord.y)/2,
        screen_tex_coord.y,
        1.0);
}
