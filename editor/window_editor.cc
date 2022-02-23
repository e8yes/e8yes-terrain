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
#include <memory>

#include "common/tensor.h"
#include "editor/component_editor_context.h"
#include "editor/component_editor_portal_switcher.h"
#include "editor/component_environment.h"
#include "editor/component_modification_monitor.h"
#include "editor/component_scene_closer.h"
#include "editor/component_scene_loader.h"
#include "editor/component_scene_saver.h"
#include "editor/component_scene_view.h"
#include "editor/component_status.h"
#include "editor/window_editor.h"

namespace e8 {

IslandsEditorWindow::IslandsEditorWindow(std::shared_ptr<EditorContext> const &editor_context,
                                         QWidget *parent)
    : QMainWindow(parent), editor_context_(editor_context) {
    editor_context_->ui->setupUi(this);

    status_comp_ = std::make_unique<StatusComponent>(editor_context.get());
    modification_monitor_comp_ =
        std::make_unique<ModificationMonitorComponent>(editor_context.get());
    editor_portal_switcher_comp_ =
        std::make_unique<EditorPortalSwitcherComponent>(editor_context.get());
    environment_comp_ = std::make_unique<EnvironmentComponent>(modification_monitor_comp_.get(),
                                                               editor_context.get());
    scene_view_comp_ = std::make_unique<SceneViewComponent>(editor_context.get());
    scene_saver_comp_ = std::make_unique<SceneSaverComponent>(
        modification_monitor_comp_.get(), scene_view_comp_.get(), editor_context.get());
    scene_closer_comp_ = std::make_unique<SceneCloserComponent>(
        editor_portal_switcher_comp_.get(), modification_monitor_comp_.get(),
        scene_saver_comp_.get(), scene_view_comp_.get(), editor_context.get());
    scene_loader_comp_ = std::make_unique<SceneLoaderComponent>(
        editor_portal_switcher_comp_.get(), environment_comp_.get(),
        modification_monitor_comp_.get(), scene_saver_comp_.get(), scene_view_comp_.get(),
        editor_context.get());
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
