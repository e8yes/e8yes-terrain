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

#ifndef ISLANDS_EDITOR_CONTEXT_H
#define ISLANDS_EDITOR_CONTEXT_H

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

#include "content/scene.h"
#include "ui_window_editor.h"

namespace e8 {

class DisplayWindowEventSource;

/**
 * @brief The EditorContext struct Stores the editor's global states.
 */
struct EditorContext {
    EditorContext();
    ~EditorContext();

    /**
     * @brief Init Initializes a new Qt application.
     */
    void Init(int &argc, char *argv[]);

    /**
     * @brief Shutdown Shut down the Qt application.
     */
    void Shutdown();

    /**
     * @brief Running Check if the Qt application is running.
     */
    bool Running();

    // The currently created Qt application.
    std::unique_ptr<QApplication> app;

    // The editor's UI.
    std::unique_ptr<Ui::IslandsEditorWindow> ui;

    // Event source that emits events coming from the display window.
    std::unique_ptr<DisplayWindowEventSource> display_window_event_source;

    // The scene the editor is currently working on, it may be null.
    std::shared_ptr<Scene> scene;

    // Indicates if the editor is running.
    bool running;
};

/**
 * @brief The DisplayWindowEventSource class Event source to connect from if any Qt object needs to
 * react to signals coming from the display window.
 */
class DisplayWindowEventSource : public QObject {
    Q_OBJECT

  public:
    DisplayWindowEventSource();
    ~DisplayWindowEventSource();

    /**
     * @brief PumpEvent Pumps display window event to the event source. It's expected to be called
     * from the display window thread.
     *
     * @param event Event to the added to the event source.
     * @param display_window The window that the event comes from.
     */
    void PumpEvent(SDL_Event const &event, SDL_Window *display_window);

  signals:
    /**
     * @brief WasdKeysTriggered Signal to emit when one of the W, A, S or D key is pressed.
     * @param key The exact key pressed, in lower case.
     */
    void WasdKeysTriggered(char key);

    /**
     * @brief MouseDragged Signal to emit when the mouse pointer is dragging on the screen.
     * @param dx The amount of horizontal distance dragged, normalized to [0, 1].
     * @param dy The amount of vertical distance dragged, normalized to [0, 1].
     */
    void MouseDragged(float dx, float dy);
};

/**
 * @brief DeepScanWidget Returns a list of Qt widget including the target as well as all of its
 * children.
 */
void DeepScanWidget(QWidget *target, std::vector<QWidget *> *result);

} // namespace e8

#endif // ISLANDS_EDITOR_CONTEXT_H
