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

#ifndef ISLANDS_EDITOR_COMPONENT_PROJECT_CREATOR_H
#define ISLANDS_EDITOR_COMPONENT_PROJECT_CREATOR_H

#include <QDialog>
#include <QObject>
#include <filesystem>
#include <memory>
#include <string>

#include "content/proto/scene.pb.h"
#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/environment/component_ambient.h"
#include "editor/environment/component_camera.h"
#include "editor/project/component_project_loader.h"
#include "editor/scene/component_scene_view.h"
#include "ui_component_project_creator.h"

namespace e8 {
namespace project_creator_internal {

struct ProjectCreationDialog : public QDialog {
    Q_OBJECT
  public:
    ProjectCreationDialog();
    ~ProjectCreationDialog();

    bool Valid() const;

  public slots:
    void OnClickBrowseButton();
    void OnAcceptInputs();

  public:
    Ui::ProjectCreationDialog ui;

    std::string project_name;
    std::filesystem::path project_directory;
    SceneProto::StructureType scene_structure;
};

} // namespace project_creator_internal

/**
 * @brief The ProjectCreatorComponent class Responsible for scene creation interactions.
 */
class ProjectCreatorComponent : public QObject {
    Q_OBJECT

  public:
    ProjectCreatorComponent(AmbientComponent *ambient_comp, CameraComponent *camera_comp,
                            EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                            ModificationMonitorComponent *modification_monitor_comp,
                            SceneViewComponent *scene_view_comp, EditorContext *context);
    ~ProjectCreatorComponent();

  public slots:
    void OnClickNewProject();

  private:
    std::unique_ptr<project_creator_internal::ProjectCreationDialog> project_creation_dialog_;

    AmbientComponent *ambient_comp_;
    CameraComponent *camera_comp_;
    EditorPortalSwitcherComponent *editor_portal_switcher_comp_;
    ModificationMonitorComponent *modification_monitor_comp_;
    SceneViewComponent *scene_view_comp_;

    EditorContext *context_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_PROJECT_CREATOR_H
