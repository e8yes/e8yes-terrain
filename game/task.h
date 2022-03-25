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

#ifndef ISLANDS_GAME_TASK_H
#define ISLANDS_GAME_TASK_H

#include "content/scene.h"
#include "renderer/renderer.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief The GameData struct Stores non-const pointers to data supporting the game.
 */
struct GameData {
    // Accessor to all of the scene's resources.
    ResourceAccessor *resource_accessor;

    // The scene the game is currently running on.
    Scene *scene;

    // Current renderer configuration.
    RendererConfiguration *renderer_config;
};

/**
 * @brief The UserInputs struct Human interaction inputs.
 */
struct UserInputs {
    // If not null, it's the key which gets pressed on the keyboard.
    char key_pressed;

    // Normalized delta movement of the mouse on the window's X axis.
    float dx;

    // Normalized delta movement of the mouse on the window's Y axis.
    float dy;

    // Indicates if the mouse's left button is pressed.
    bool left_mouse_button_pressed;

    // Indicates if the mouse's right button is pressed.
    bool right_mouse_button_pressed;
};

/**
 * @brief The TaskInterface class Represents a task which needs to be run per-frame to support the
 * game.
 */
class TaskInterface {
  public:
    TaskInterface();
    virtual ~TaskInterface();

    /**
     * @brief OnFrame Logic/task to run as the frame starts.
     */
    virtual void OnFrame(UserInputs const &system_inputs, GameData *game_data) = 0;
};

} // namespace e8

#endif // ISLANDS_GAME_TASK_H
