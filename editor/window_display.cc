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

#include "editor/component_scene.h"
#include "editor/window_display.h"
#include "renderer/context.h"
#include "renderer/renderer_solid_color.h"

namespace e8 {
namespace {

constexpr char const *kIslandsDisplayWindowTitle = "e8 islands display";

void RunDisplayLoop(std::shared_ptr<EditorContext> const &editor_context,
                    SolidColorRenderer *renderer) {
    while (editor_context->running) {
        if (editor_context->scene != nullptr) {
            renderer->DrawFrame(editor_context->scene.get());
        }

        SDL_Event event;
        if (!SDL_PollEvent(&event)) {
            continue;
        }

        switch (event.type) {
        case SDL_QUIT: {
            break;
        }
        }
    }
}

} // namespace

void RunIslandsDisplay(std::shared_ptr<EditorContext> editor_context, unsigned window_width,
                       unsigned window_height) {
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

        RunDisplayLoop(editor_context, &renderer);
    }

    SDL_DestroyWindow(display_window);
}

} // namespace e8
