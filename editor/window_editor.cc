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

#include <QKeyEvent>
#include <QMainWindow>
#include <QWidget>
#include <memory>

#include "common/tensor.h"
#include "editor/component_editor_context.h"
#include "editor/component_environment.h"
#include "editor/component_scene.h"
#include "editor/window_editor.h"

namespace e8 {

IslandsEditorWindow::IslandsEditorWindow(std::shared_ptr<EditorContext> const &editor_context,
                                         QWidget *parent)
    : QMainWindow(parent), editor_context_(editor_context) {
    editor_context_->ui->setupUi(this);
    environment_component_ = std::make_unique<EnvironmentComponent>(editor_context.get());
    scene_component_ =
        std::make_unique<SceneComponent>(environment_component_.get(), editor_context.get());
}

IslandsEditorWindow::~IslandsEditorWindow() {}

void RunIslandsEditorWindow(std::shared_ptr<EditorContext> editor_context, int argc, char *argv[]) {
    QApplication application(argc, argv);

    IslandsEditorWindow editor_window(editor_context);
    editor_window.show();

    application.exec();

    editor_context->running = false;
}

} // namespace e8
