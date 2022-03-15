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

#include <QApplication>
#include <QList>
#include <QObject>
#include <QWidget>
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

#include "content/scene.h"
#include "editor/basic/context.h"
#include "ui_window_editor.h"

namespace e8 {

EditorContext::EditorContext() : running(false) {}

EditorContext::~EditorContext() {}

void EditorContext::Init(int &argc, char *argv[]) {
    app = std::make_unique<QApplication>(argc, argv);
    ui = std::make_unique<Ui::IslandsEditorWindow>();
    display_window_event_source = std::make_unique<DisplayWindowEventSource>();
    running = true;
}

void EditorContext::Shutdown() {
    app = nullptr;
    running = false;
}

bool EditorContext::Running() { return running; }

void DeepScanWidget(QWidget *target, std::vector<QWidget *> *result) {
    if (target == nullptr) {
        return;
    }

    result->push_back(target);

    QList<QWidget *> children = target->findChildren<QWidget *>();
    for (auto child : children) {
        result->push_back(child);
    }
}

DisplayWindowEventSource::DisplayWindowEventSource() {}

DisplayWindowEventSource::~DisplayWindowEventSource() {}

void DisplayWindowEventSource::PumpEvent(SDL_Event const &event, SDL_Window *display_window) {
    switch (event.type) {
    case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case 'w':
        case 'a':
        case 's':
        case 'd':
            emit WasdKeysTriggered(static_cast<char>(event.key.keysym.sym));
            break;
        }
        break;
    }
    }

    if (event.motion.state & SDL_BUTTON_LMASK) {
        int width;
        int height;
        SDL_GL_GetDrawableSize(display_window, &width, &height);

        float dx = 0;
        float dy = 0;
        if (std::abs(event.motion.xrel) < width) {
            dx = static_cast<float>(event.motion.xrel) / width;
        }
        if (std::abs(event.motion.yrel) < height) {
            dy = static_cast<float>(event.motion.yrel) / height;
        }

        emit MouseDragged(dx, dy);
    }
}

} // namespace e8
