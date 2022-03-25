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

#include <algorithm>
#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "game/storyline.h"
#include "game/task.h"

namespace e8 {

StoryComponentInterface::StoryComponentInterface() {}

StoryComponentInterface::~StoryComponentInterface() {}

void StoryComponentInterface::OnTransition(StoryComponentStatus const & /*criteria*/) {}

std::optional<StoryComponentStatus> StoryComponentInterface::CompletionStatus() {
    return std::nullopt;
}

Storyline::Storyline() {}

Storyline::~Storyline() {}

void Storyline::AddComponent(StoryComponentId const &id,
                             std::unique_ptr<StoryComponentInterface> &&component) {
    story_comps_[id] = std::move(component);
}

StoryComponentInterface *Storyline::FindComponent(StoryComponentId const &id) {
    auto it = story_comps_.find(id);
    if (it == story_comps_.end()) {
        return nullptr;
    }
    return it->second.get();
}

void Storyline::StartFrom(StoryComponentId const &component) { current_component_ = component; }

void Storyline::Transition(StoryComponentId const &component_a,
                           StoryComponentStatus const &criteria,
                           StoryComponentId const &component_b) {
    transitions_[component_a][criteria] = component_b;
}

void Storyline::ProcessFrame(UserInputs const &user_inputs, GameData *game_data) {
    auto comp_it = story_comps_.find(current_component_);
    assert(comp_it != story_comps_.end());

    for (auto &task : comp_it->second->tasks) {
        task->OnFrame(user_inputs, game_data);
    }

    std::optional<StoryComponentStatus> complete_status = comp_it->second->CompletionStatus();
    if (!complete_status.has_value()) {
        return;
    }

    auto transition_it = transitions_.find(current_component_);
    assert(transition_it != transitions_.end());

    auto target_it = transition_it->second.find(*complete_status);
    assert(target_it != transition_it->second.end());

    current_component_ = target_it->second;
}

} // namespace e8
