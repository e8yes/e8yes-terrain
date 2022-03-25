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

#ifndef ISLANDS_GAME_DISPLAY_H
#define ISLANDS_GAME_DISPLAY_H

#include <SDL2/SDL.h>

namespace e8 {

/**
 * @brief The Display struct An OS window for displaying rendering result.
 */
struct Display {
    /**
     * @brief Display Constructing a display window with the specified dimension.
     */
    Display(unsigned width, unsigned height);
    Display(Display const &) = delete;
    ~Display();

    // The display window.
    SDL_Window *window;
};

} // namespace e8

#endif // ISLANDS_GAME_DISPLAY_H
