/**
Basic FXAA implementation based on the code on geeks3d.com with the
modification that the textureLod stuff was removed since it's
unsupported by WebGL.
--
From:
https://github.com/mitsuhiko/webgl-meincraft
Copyright (c) 2011 by Armin Ronacher.
Some rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * The names of the contributors may not be used to endorse or
      promote products derived from this software without specific
      prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

const float FXAA_REDUCE_MIN = 1.0/256.0;
const float FXAA_REDUCE_MUL = 1.0/8.0;
const float FXAA_SPAN_MAX = 8.0;

float ToLuminance(vec3 color) {
    const vec3 to_luma = vec3(0.299, 0.587, 0.114);
    return dot(color, to_luma);
}

vec3 Fxaa(sampler2D tex,
          vec2 tex_coord,
          vec2 screen_resolution) {
    vec2 inv_view_port = 1.0/screen_resolution;

    vec2 tex_coord_nw = vec2(tex_coord.x - inv_view_port.x, tex_coord.y - inv_view_port.y);
    vec2 tex_coord_ne = vec2(tex_coord.x + inv_view_port.x, tex_coord.y - inv_view_port.y);
    vec2 tex_coord_sw = vec2(tex_coord.x - inv_view_port.x, tex_coord.y + inv_view_port.y);
    vec2 tex_coord_se = vec2(tex_coord.x + inv_view_port.x, tex_coord.y + inv_view_port.y);

    vec3 color_nw = texture(tex, tex_coord_nw).xyz;
    vec3 color_ne = texture(tex, tex_coord_ne).xyz;
    vec3 color_sw = texture(tex, tex_coord_sw).xyz;
    vec3 color_se = texture(tex, tex_coord_se).xyz;
    vec3 color = texture(tex, tex_coord).xyz;

    float luma_nw = ToLuminance(color_nw);
    float luma_ne = ToLuminance(color_ne);
    float luma_sw = ToLuminance(color_sw);
    float luma_se = ToLuminance(color_se);
    float luma = ToLuminance(color);

    vec2 dir;
    dir.x = -((luma_nw + luma_ne) - (luma_sw + luma_se));
    dir.y = (luma_nw + luma_sw) - (luma_ne + luma_se);

    float dir_reduce = max((luma_nw + luma_ne + luma_sw + luma_se)*
                           (0.25*FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

    float inv_dir_min = 1.0/(min(abs(dir.x), abs(dir.y)) + dir_reduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
              max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
              dir * inv_dir_min)) * inv_view_port;

    vec3 color_a = 0.5*(
        texture(tex, tex_coord + dir*(1.0/3.0 - 0.5)).xyz +
        texture(tex, tex_coord + dir*(2.0/3.0 - 0.5)).xyz);
    vec3 color_b = color_a*0.5 + 0.25*(
        texture(tex, tex_coord + dir*-0.5).xyz +
        texture(tex, tex_coord + dir*0.5).xyz);

    float luma_b = ToLuminance(color_b);
    float luma_min = min(luma, min(min(luma_nw, luma_ne), min(luma_sw, luma_se)));
    float luma_max = max(luma, max(max(luma_nw, luma_ne), max(luma_sw, luma_se)));

    if (luma_b < luma_min || luma_b > luma_max) {
        return color_a;
    } else {
        return color_b;
    }
}
