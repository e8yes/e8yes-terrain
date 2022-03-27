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

#include <QObject>
#include <cassert>
#include <memory>

#include "content/camera.h"
#include "content/scene.h"
#include "editor/basic/editor_storyline.h"
#include "game/storyline.h"
#include "game/task.h"

namespace e8 {
namespace {

unsigned const kRendererPerformanceUpdateInterval = 30;

enum MoveDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

void MoveSceneCamera(MoveDirection direction, float sensitivity, Scene *scene) {
    Scene::WriteAccess write_access = scene->GainWriteAccess();

    Camera::Basis basis = scene->camera.basis();
    vec3 camera_position = ToVec3(scene->camera.position());

    switch (direction) {
    case FORWARD: {
        camera_position = camera_position - sensitivity * ToVec3(basis.back());
        break;
    }
    case BACKWARD: {
        camera_position = camera_position + sensitivity * ToVec3(basis.back());
        break;
    }
    case LEFT: {
        camera_position = camera_position - sensitivity * ToVec3(basis.right());
        break;
    }
    case RIGHT: {
        camera_position = camera_position + sensitivity * ToVec3(basis.right());
        break;
    }
    }

    *scene->camera.mutable_position() = ToProto(camera_position);
}

void RotateSceneCamera(float dx, float dy, float sensitivity, Scene *scene) {
    Scene::WriteAccess write_access = scene->GainWriteAccess();
    CameraRotateBasis(/*roll=*/0, dy * sensitivity, -dx * sensitivity, &scene->camera);
    CameraEnsureLeveling(&scene->camera);
}

} // namespace

EditorCameraEventTransmitter::EditorCameraEventTransmitter() {}

EditorCameraEventTransmitter::~EditorCameraEventTransmitter() {}

void EditorCameraEventTransmitter::Notify() { emit ValueChanged(); }

EditorCameraControlTask::EditorCameraControlTask()
    : rotation_sensitivity(50.0f), movement_sensitivity(0.1f) {}

EditorCameraControlTask::~EditorCameraControlTask() {}

void EditorCameraControlTask::OnFrame(UserInputs const &user_inputs, GameData *game_data) {
    bool camera_changed = false;

    switch (user_inputs.key_pressed) {
    case 'w':
        MoveSceneCamera(MoveDirection::FORWARD, movement_sensitivity, game_data->scene);
        camera_changed = true;
        break;
    case 'a': {
        MoveSceneCamera(MoveDirection::LEFT, movement_sensitivity, game_data->scene);
        camera_changed = true;
        break;
    }
    case 's': {
        MoveSceneCamera(MoveDirection::BACKWARD, movement_sensitivity, game_data->scene);
        camera_changed = true;
        break;
    }
    case 'd': {
        MoveSceneCamera(MoveDirection::RIGHT, movement_sensitivity, game_data->scene);
        camera_changed = true;
        break;
    }
    }

    if (user_inputs.left_mouse_button_pressed) {
        RotateSceneCamera(user_inputs.dx, user_inputs.dy, rotation_sensitivity, game_data->scene);
        camera_changed = true;
    }

    if (camera_changed) {
        transmitter.Notify();
    }
}

RendererPerformanceUpdateEventTransmitter::RendererPerformanceUpdateEventTransmitter() {}

RendererPerformanceUpdateEventTransmitter::~RendererPerformanceUpdateEventTransmitter() {}

void RendererPerformanceUpdateEventTransmitter::Notify() { emit UpdateRequired(); }

RendererPerformanceUpdateTask::RendererPerformanceUpdateTask() : frame_counter(0) {}

RendererPerformanceUpdateTask::~RendererPerformanceUpdateTask() {}

void RendererPerformanceUpdateTask::OnFrame(UserInputs const & /*user_inputs*/,
                                            GameData * /*game_data*/) {
    if (frame_counter % kRendererPerformanceUpdateInterval == 0) {
        transmitter.Notify();
    }

    ++frame_counter;
}

EditorStoryComponent::EditorStoryComponent() {
    tasks.push_back(std::make_unique<EditorCameraControlTask>());
    tasks.push_back(std::make_unique<RendererPerformanceUpdateTask>());
}

EditorStoryComponent::~EditorStoryComponent() {}

EditorCameraControlTask *EditorStoryComponent::GetCameraControlTask() {
    return static_cast<EditorCameraControlTask *>(tasks[0].get());
}

RendererPerformanceUpdateTask *EditorStoryComponent::GetRendererPerformanceUpdateTask() {
    return static_cast<RendererPerformanceUpdateTask *>(tasks[1].get());
}

std::unique_ptr<Storyline> CreateEditorStoryline() {
    auto storyline = std::make_unique<Storyline>();
    storyline->AddComponent(kEditorComponent, std::make_unique<EditorStoryComponent>());
    storyline->StartFrom(kEditorComponent);
    return storyline;
}

} // namespace e8
