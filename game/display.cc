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

#include <SDL2/SDL.h>
#include <cassert>

#include "game/display.h"

namespace e8 {
namespace {

constexpr char const *kIslandsDisplayWindowTitle = "e8 islands display";

} // namespace

Display::Display(unsigned width, unsigned height) {
    assert(0 == SDL_Init(SDL_INIT_VIDEO));

    window = SDL_CreateWindow(
        /*title=*/kIslandsDisplayWindowTitle, /*x=*/SDL_WINDOWPOS_UNDEFINED,
        /*y=*/SDL_WINDOWPOS_UNDEFINED, width, height,
        /*flags=*/SDL_WINDOW_VULKAN);
    assert(window != nullptr);
}

Display::~Display() { SDL_DestroyWindow(window); }

} // namespace e8
