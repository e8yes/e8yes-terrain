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

#include <QMainWindow>
#include <QWidget>
#include <algorithm>
#include <memory>

#include "common/tensor.h"
#include "editor/window_editor.h"
#include "ui_window_editor.h"

namespace e8 {

IslandsEditorWindow::IslandsEditorWindow(SceneInterface *scene, QWidget *parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::IslandsEditorWindow>()), scene_(scene) {
    ui_->setupUi(this);
}

IslandsEditorWindow::~IslandsEditorWindow() {}

void IslandsEditorWindow::keyPressEvent(QKeyEvent *event) {
    vec3 current_color = scene_->BackgroundColor();

    switch (event->key()) {
    case Qt::Key_Up: {
        current_color(0) = std::min(1.0f, current_color(0) + 0.01f);
        break;
    }
    case Qt::Key_Down: {
        current_color(0) = std::max(0.0f, current_color(0) - 0.01f);
        break;
    }
    case Qt::Key_Right: {
        current_color(1) = std::min(1.0f, current_color(1) + 0.01f);
        break;
    }
    case Qt::Key_Left: {
        current_color(1) = std::max(0.0f, current_color(1) - 0.01f);
        break;
    }
    case Qt::Key_PageUp: {
        current_color(2) = std::min(1.0f, current_color(2) + 0.01f);
        break;
    }
    case Qt::Key_PageDown: {
        current_color(2) = std::max(0.0f, current_color(2) - 0.01f);
        break;
    }
    default: {
        break;
    }
    }

    scene_->UpdateBackgroundColor(current_color);
}

void RunIslandsEditorWindow(SceneInterface *scene, int argc, char *argv[]) {
    QApplication a(argc, argv);

    IslandsEditorWindow w(scene);
    w.show();

    a.exec();
}

} // namespace e8
