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

#ifndef ISLANDS_EDITOR_H
#define ISLANDS_EDITOR_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QWidget>
#include <memory>

#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/component_status.h"
#include "editor/basic/context.h"
#include "editor/display/component_renderer.h"
#include "editor/environment/component_ambient.h"
#include "editor/environment/component_camera.h"
#include "editor/object/component_scene_object_gltf.h"
#include "editor/procedural/component_procedural_plane.h"
#include "editor/project/component_project_closer.h"
#include "editor/project/component_project_creator.h"
#include "editor/project/component_project_loader.h"
#include "editor/project/component_project_saver.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {

/**
 * @brief The IslandsEditorWindow class The editor panel.
 */
class IslandsEditorWindow : public QMainWindow {
    Q_OBJECT

  public:
    IslandsEditorWindow(EditorContext *editor_context, QWidget *parent = nullptr);
    ~IslandsEditorWindow();

    void closeEvent(QCloseEvent *) override;

  private:
    std::unique_ptr<StatusComponent> status_comp_;
    std::unique_ptr<ModificationMonitorComponent> modification_monitor_comp_;
    std::unique_ptr<EditorPortalSwitcherComponent> editor_portal_switcher_comp_;
    std::unique_ptr<AmbientComponent> ambient_comp_;
    std::unique_ptr<CameraComponent> camera_comp_;
    std::unique_ptr<RendererComponent> renderer_comp_;
    std::unique_ptr<SceneViewComponent> scene_view_comp_;
    std::unique_ptr<ProjectSaverComponent> project_saver_comp_;
    std::unique_ptr<ProjectCloserComponent> project_closer_comp_;
    std::unique_ptr<ProjectCreatorComponent> project_creator_comp_;
    std::unique_ptr<ProjectLoaderComponent> project_loader_comp_;
    std::unique_ptr<SceneObjectGltfComponent> scene_object_gltf_comp_;
    std::unique_ptr<ProceduralPlaneComponent> procedural_plane_comp_;
};

/**
 * @brief RunEditor Runs the editor window. This function blocks until the editor
 * window is closed.
 */
int RunEditor(int argc, char *argv[]);

} // namespace e8

#endif // ISLANDS_EDITOR_H
