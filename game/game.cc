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
#include <cmath>
#include <memory>
#include <mutex>
#include <string>

#include "common/device.h"
#include "content/scene.h"
#include "game/display.h"
#include "game/game.h"
#include "game/storyline.h"
#include "game/task.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "resource/accessor.h"
#include "resource/table.h"

namespace e8 {
namespace {

unsigned const kDisplayWindowWidth = 1280;
unsigned const kDisplayWindowHeight = 800;

void UpdateSystemInputs(Display const &display, UserInputs *system_inputs) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN: {
            system_inputs->key_pressed = event.key.keysym.sym;
            break;
        }
        case SDL_KEYUP: {
            system_inputs->key_pressed = '\0';
            break;
        }
        case SDL_MOUSEMOTION: {
            int width;
            int height;
            SDL_GL_GetDrawableSize(display.window, &width, &height);

            system_inputs->dx = 0;
            system_inputs->dy = 0;
            if (std::abs(event.motion.xrel) < width) {
                system_inputs->dx = static_cast<float>(event.motion.xrel) / width;
            }
            if (std::abs(event.motion.yrel) < height) {
                system_inputs->dy = static_cast<float>(event.motion.yrel) / height;
            }

            system_inputs->left_mouse_button_pressed = event.motion.state & SDL_BUTTON_LMASK;
            system_inputs->right_mouse_button_pressed = event.motion.state & SDL_BUTTON_RMASK;
            break;
        }
        }
    }
}

} // namespace

Game::Game(std::filesystem::path const &base_path, SceneProto::StructureType scene_structure,
           std::string const &scene_name)
    : base_path_(base_path), done_(false) {
    display_ = std::make_unique<Display>(kDisplayWindowWidth, kDisplayWindowHeight);
    device_context_ = CreateVulkanContext(display_->window);
    resource_accessor_ = std::make_unique<ResourceAccessor>(base_path, /*load_existing=*/false,
                                                            device_context_.get());
    scene_ = std::make_unique<Scene>(scene_structure, scene_name);
    renderer_config_ = DefaultRendererConfiguration();

    assert(this->Valid());
}

Game::Game(std::filesystem::path const &base_path) : base_path_(base_path), done_(false) {
    display_ = std::make_unique<Display>(kDisplayWindowWidth, kDisplayWindowHeight);
    device_context_ = CreateVulkanContext(display_->window);
    resource_accessor_ = std::make_unique<ResourceAccessor>(base_path, /*load_existing=*/true,
                                                            device_context_.get());
    if (!resource_accessor_->Valid()) {
        return;
    }

    scene_ = LoadScene(base_path, resource_accessor_.get());
    if (scene_ == nullptr) {
        return;
    }

    renderer_config_ = LoadRendererConfiguration(base_path);
    if (renderer_config_ == nullptr) {
        return;
    }

    assert(this->Valid());
}

Game::~Game() {}

bool Game::Valid() const {
    return resource_accessor_->Valid() && scene_ != nullptr && renderer_config_ != nullptr;
}

void Game::Run(Storyline *storyline) {
    assert(display_ != nullptr);
    assert(device_context_ != nullptr);
    assert(resource_accessor_ != nullptr);
    assert(scene_ != nullptr);
    assert(renderer_config_ != nullptr);

    RendererConfiguration current_renderer_config;

    while (!done_) {
        if (renderer_ == nullptr || renderer_config_->in_use_renderer_type() !=
                                        current_renderer_config.in_use_renderer_type()) {
            current_renderer_config.set_in_use_renderer_type(
                renderer_config_->in_use_renderer_type());
            renderer_ = CreateRenderer(current_renderer_config.in_use_renderer_type(),
                                       device_context_.get());
        }

        if (renderer_config_->DebugString() != current_renderer_config.DebugString()) {
            current_renderer_config = *renderer_config_;
            renderer_->ApplyConfiguration(current_renderer_config);
        }

        renderer_->DrawFrame(scene_.get(), resource_accessor_.get());

        UserInputs user_inputs;
        UpdateSystemInputs(*display_, &user_inputs);
        GameData game_data = this->GetGameData();
        storyline->ProcessFrame(user_inputs, &game_data);
    }

    done_ = false;
}

void Game::Shutdown() { done_ = true; }

bool Game::Save() {
    if (!resource_accessor_->Commit()) {
        return false;
    }

    if (!SaveScene(*scene_, base_path_)) {
        return false;
    }

    if (!SaveRendererConfiguration(*renderer_config_, base_path_)) {
        return false;
    }

    return true;
}

GameData Game::GetGameData() {
    GameData game_data;
    game_data.resource_accessor = resource_accessor_.get();
    game_data.scene = scene_.get();
    game_data.renderer_config = renderer_config_.get();
    game_data.renderer = renderer_.get();
    return game_data;
}

} // namespace e8
