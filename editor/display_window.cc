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
#include <memory>

#include "content/scene.h"
#include "editor/display_window.h"
#include "renderer/context.h"
#include "renderer/renderer_solid_color.h"

namespace e8 {
namespace {

constexpr char const *kIslandsDisplayWindowTitle = "e8 islands display";

void RunDisplayLoop(SceneInterface const *scene, SolidColorRenderer *renderer, bool *quit_display) {
    while (!*quit_display) {
        renderer->DrawFrame(*scene);

        SDL_Event event;
        if (!SDL_PollEvent(&event)) {
            continue;
        }

        switch (event.type) {}
    }
}

} // namespace

void RunIslandsDisplay(SceneInterface const *scene, unsigned window_width, unsigned window_height,
                       bool *quit_display) {
    assert(0 == SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *display_window = SDL_CreateWindow(
        /*title=*/kIslandsDisplayWindowTitle, /*x=*/SDL_WINDOWPOS_UNDEFINED,
        /*y=*/SDL_WINDOWPOS_UNDEFINED, window_width, window_height,
        /*flags=*/SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_VULKAN);
    assert(display_window != nullptr);

    {
        // Initializes a renderer and renders the scene state continually.
        std::unique_ptr<VulkanContext> context = CreateVulkanContext(display_window);
        SolidColorRenderer renderer(context.get());

        RunDisplayLoop(scene, &renderer, quit_display);
    }

    SDL_DestroyWindow(display_window);
}

} // namespace e8
