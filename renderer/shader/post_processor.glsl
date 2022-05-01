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

layout(set = 0, binding = 0) uniform PerFrameConstants {
    float viewport_width;
    float viewport_height;
} pfc;

vec2 ScreenTexCoord() {
    return vec2(gl_FragCoord.x / pfc.viewport_width, gl_FragCoord.y / pfc.viewport_height);
}

vec2 ScreenResolution() {
    return vec2(pfc.viewport_width, pfc.viewport_height);
}
