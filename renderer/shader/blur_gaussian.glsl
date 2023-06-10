/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLUR_GAUSSIAN_GLSL
#define BLUR_GAUSSIAN_GLSL

#include "post_processor.glsl"

layout(set = 1, binding = 0) uniform sampler2D source_image;

float GaussianBlurHorizontal1x1Float() {
    vec2 uv = ScreenTexCoord();
    return texture(source_image, uv).x;
}

float GaussianBlurVertical1x1Float() {
    return GaussianBlurHorizontal1x1Float();
}

#endif // BLUR_GAUSSIAN_GLSL
