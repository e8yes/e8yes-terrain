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

#ifndef ISLANDS_GAME_H
#define ISLANDS_GAME_H

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>

#include "common/device.h"
#include "content/proto/scene.pb.h"
#include "content/scene.h"
#include "game/display.h"
#include "game/storyline.h"
#include "game/task.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief The Game class Encapsulates all information a game requires.
 */
class Game {
  public:
    /**
     * @brief Game Constructs a new empty game.
     *
     * @param base_path The "root" directory under which game data are stored.
     * @param scene_structure The scene structure used with the game.
     * @param scene_name A descriptive human readable name for the scene.
     */
    Game(std::filesystem::path const &base_path, SceneProto::StructureType scene_structure,
         std::string const &scene_name);

    /**
     * @brief Game Loads an existing game. Use the Game::Valid() function to check if the game is
     * loaded successfully.
     *
     * @param base_path The "root" directory under which game data are stored.
     */
    Game(std::filesystem::path const &base_path);

    Game(Game const &) = delete;
    ~Game();

    /**
     * @brief Valid Checks if the game data are properly loaded.
     */
    bool Valid() const;

    /**
     * @brief Run Runs the game with the specified storyline. This function blocks until
     * Game::Shutdown() is called in another thread.
     */
    void Run(Storyline *storyline);

    /**
     * @brief Shutdown Signals the Game::Run() call to terminate.
     */
    void Shutdown();

    /**
     * @brief Save Saves all game data to disk.
     */
    bool Save();

    /**
     * @brief GetGameData Returns the current game data.
     */
    GameData GetGameData();

  private:
    std::filesystem::path base_path_;

    std::unique_ptr<Display> display_;
    std::unique_ptr<VulkanContext> device_context_;
    std::unique_ptr<ResourceAccessor> resource_accessor_;
    std::unique_ptr<Scene> scene_;
    std::unique_ptr<RendererConfiguration> renderer_config_;
    std::unique_ptr<RendererInterface> renderer_;

    bool done_;
    std::mutex mu_;
};

} // namespace e8

#endif // ISLANDS_GAME_H
