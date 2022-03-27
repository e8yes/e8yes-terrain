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

#ifndef ISLANDS_EDITOR_STORYLINE_H
#define ISLANDS_EDITOR_STORYLINE_H

#include <QObject>
#include <memory>

#include "game/storyline.h"
#include "game/task.h"

namespace e8 {

// It's the only story component in the editor storyline.
StoryComponentId const kEditorComponent = StoryComponentId("Editor Interaction");

/**
 * @brief The EditorCameraEventTransmitter class Allows the editor UI to be notified as the camera
 * pose changes by editor interactions occurred in the display window.
 */
class EditorCameraEventTransmitter : public QObject {
    Q_OBJECT

  public:
    EditorCameraEventTransmitter();
    ~EditorCameraEventTransmitter();

  public:
    void Notify();

  signals:
    void ValueChanged();
};

/**
 * @brief The EditorCameraControlTask class A task to handle editor interaction (FPS-style camera
 * control).
 */
struct EditorCameraControlTask : public TaskInterface {
    EditorCameraControlTask();
    ~EditorCameraControlTask();

    void OnFrame(UserInputs const &user_inputs, GameData *game_data) override;

    EditorCameraEventTransmitter transmitter;
    float rotation_sensitivity;
    float movement_sensitivity;
};

/**
 * @brief The RendererPerformanceUpdateEventTransmitter class Allows the editor UI to be notified
 * when renderer performance statistics accumulates enough changes.
 */
class RendererPerformanceUpdateEventTransmitter : public QObject {
    Q_OBJECT

  public:
    RendererPerformanceUpdateEventTransmitter();
    ~RendererPerformanceUpdateEventTransmitter();

  public:
    void Notify();

  signals:
    void UpdateRequired();
};

/**
 * @brief The RendererPerformanceUpdateTask struct A task to keep the UI showing renderer
 * performance statistics in a regular interval.
 */
struct RendererPerformanceUpdateTask : public TaskInterface {
    RendererPerformanceUpdateTask();
    ~RendererPerformanceUpdateTask();

    void OnFrame(UserInputs const &user_inputs, GameData *game_data) override;

    RendererPerformanceUpdateEventTransmitter transmitter;
    unsigned frame_counter;
};

/**
 * @brief The EditorStoryComponent struct The only story component in the editor storyline. It's
 * responsible for running editor tasks.
 */
struct EditorStoryComponent : public StoryComponentInterface {
    EditorStoryComponent();
    ~EditorStoryComponent();

    EditorCameraControlTask *GetCameraControlTask();

    RendererPerformanceUpdateTask *GetRendererPerformanceUpdateTask();
};

/**
 * @brief CreateEditorStoryline It's technically not a storyline. It uses the storyline construct to
 * implment some of the editor interaction logic. This storyline runs in the game thread.
 */
std::unique_ptr<Storyline> CreateEditorStoryline();

} // namespace e8

#endif // ISLANDS_EDITOR_STORYLINE_H
