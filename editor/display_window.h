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

#ifndef ISLANDS_EDITOR_DISPLAY_WINDOW_H
#define ISLANDS_EDITOR_DISPLAY_WINDOW_H

namespace e8 {

/**
 * @brief RunIslandsDisplay Creates and runs a display task for visual presentation of the editor's
 * current scene state. Note, this function blocks until quit_display is set to true.
 *
 * @param window_width The width of the display window.
 * @param window_height The height of the display window.
 * @param quit_display Allows to be set by a different thread to close the display.
 */
void RunIslandsDisplay(unsigned window_width, unsigned window_height, bool *quit_display);

} // namespace e8

#endif // ISLANDS_EDITOR_DISPLAY_WINDOW_H
