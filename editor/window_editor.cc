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

#include <QCloseEvent>
#include <QMainWindow>
#include <QWidget>
#include <memory>

#include "common/tensor.h"
#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/component_status.h"
#include "editor/basic/context.h"
#include "editor/environment/component_ambient.h"
#include "editor/environment/component_camera.h"
#include "editor/object/component_scene_object_gltf.h"
#include "editor/procedural/component_procedural_plane.h"
#include "editor/scene/component_scene_closer.h"
#include "editor/scene/component_scene_loader.h"
#include "editor/scene/component_scene_saver.h"
#include "editor/scene/component_scene_view.h"
#include "editor/window_editor.h"

namespace e8 {

IslandsEditorWindow::IslandsEditorWindow(EditorContext *editor_context, QWidget *parent)
    : QMainWindow(parent) {
    editor_context->ui->setupUi(this);

    status_comp_ = std::make_unique<StatusComponent>(this, editor_context);
    modification_monitor_comp_ =
        std::make_unique<ModificationMonitorComponent>(status_comp_.get(), editor_context);
    editor_portal_switcher_comp_ = std::make_unique<EditorPortalSwitcherComponent>(editor_context);
    ambient_comp_ =
        std::make_unique<AmbientComponent>(modification_monitor_comp_.get(), editor_context);
    camera_comp_ =
        std::make_unique<CameraComponent>(modification_monitor_comp_.get(), editor_context);
    scene_view_comp_ = std::make_unique<SceneViewComponent>(editor_context);
    scene_saver_comp_ = std::make_unique<SceneSaverComponent>(
        modification_monitor_comp_.get(), scene_view_comp_.get(), editor_context);
    scene_closer_comp_ = std::make_unique<SceneCloserComponent>(
        editor_portal_switcher_comp_.get(), modification_monitor_comp_.get(),
        scene_saver_comp_.get(), scene_view_comp_.get(), editor_context);
    scene_loader_comp_ = std::make_unique<SceneLoaderComponent>(
        ambient_comp_.get(), camera_comp_.get(), editor_portal_switcher_comp_.get(),
        modification_monitor_comp_.get(), scene_view_comp_.get(), editor_context);
    scene_object_gltf_comp_ = std::make_unique<SceneObjectGltfComponent>(
        modification_monitor_comp_.get(), scene_view_comp_.get(), editor_context);
    procedural_plane_comp_ = std::make_unique<ProceduralPlaneComponent>(
        modification_monitor_comp_.get(), scene_view_comp_.get(), editor_context);

    QAction::connect(editor_context->ui->action_exit, &QAction::triggered, this,
                     &IslandsEditorWindow::close);
}

IslandsEditorWindow::~IslandsEditorWindow() {}

void IslandsEditorWindow::closeEvent(QCloseEvent *) { scene_closer_comp_->OnClickCloseScene(); }

int RunEditor(int argc, char *argv[]) {
    auto editor_context = std::make_unique<EditorContext>(argc, argv);

    int ret_val;
    {
        IslandsEditorWindow editor_window(editor_context.get());
        editor_window.show();
        ret_val = editor_context->app->exec();
    }

    return ret_val;
}

} // namespace e8
