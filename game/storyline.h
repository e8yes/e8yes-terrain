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

#ifndef ISLANDS_GAME_STORYLINE_H
#define ISLANDS_GAME_STORYLINE_H

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "game/task.h"

namespace e8 {

// ID of a story component.
using StoryComponentId = std::string;

// The story component's completion status.
using StoryComponentStatus = std::string;

/**
 * @brief The StoryComponentInterface class Represents a piece of story.
 */
class StoryComponentInterface {
  public:
    StoryComponentInterface();
    virtual ~StoryComponentInterface();

    /**
     * @brief OnTransition This gets called upon the transition.
     * @param criteria The status which the previous story component completes with.
     */
    virtual void OnTransition(StoryComponentStatus const &criteria);

    /**
     * @brief CompletionStatus If the story component is complete, it must return a status so that
     * the Storyline knows which story component it should transition to.
     */
    virtual std::optional<StoryComponentStatus> CompletionStatus();

  public:
    // Tasks that need to be run each frame.
    std::vector<std::unique_ptr<TaskInterface>> tasks;
};

/**
 * @brief The Storyline class Represents a game's storyline with inter-connecting story components
 * (a directed graph).
 */
class Storyline {
  public:
    Storyline();
    Storyline(Storyline const &) = delete;
    ~Storyline();

    /**
     * @brief AddComponent Adds a new story component. The storyline will own the new component.
     */
    void AddComponent(StoryComponentId const &id,
                      std::unique_ptr<StoryComponentInterface> &&component);

    /**
     * @brief FindComponent Looks up the story component by ID, if it exists. Otherwise, it returns
     * a nullptr.
     */
    StoryComponentInterface *FindComponent(StoryComponentId const &id);

    /**
     * @brief StartFrom Marks the starting story component. If the starting component was set, this
     * call will override the setup.
     */
    void StartFrom(StoryComponentId const &component);

    /**
     * @brief Transition Defines a transition between two story components. If a previous transition
     * exists, it will override the criteria.
     *
     * @param component_a The component to transition from.
     * @param criteria The completion status with which component_a must complete to make this
     * transition happen.
     * @param component_b The component to transition to.
     */
    void Transition(StoryComponentId const &component_a, StoryComponentStatus const &criteria,
                    StoryComponentId const &component_b);

    /**
     * @brief ProcessFrame Runs the current story component's tasks and transitions to the next
     * component when it finishes with a status. Storyline::StartFrom() must be called before
     * calling this function, or else, this function will fail.
     */
    void ProcessFrame(UserInputs const &user_inputs, GameData *game_data);

  private:
    std::unordered_map<StoryComponentId, std::unique_ptr<StoryComponentInterface>> story_comps_;
    std::unordered_map<StoryComponentId, std::unordered_map<StoryComponentStatus, StoryComponentId>>
        transitions_;

    StoryComponentId current_component_;
};

} // namespace e8

#endif // ISLANDS_GAME_STORYLINE_H
