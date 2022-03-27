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

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <memory>
#include <string>

#include "content/proto/scene.pb.h"
#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/basic/theme.h"
#include "editor/environment/component_ambient.h"
#include "editor/environment/component_camera.h"
#include "editor/project/component_project_creator.h"
#include "editor/project/component_project_loader.h"
#include "editor/scene/component_scene_view.h"
#include "game/game.h"
#include "ui_component_project_creator.h"

namespace e8 {
namespace {

constexpr char const *kDefaultSceneName = "Untitled";

bool CreateNewProject(std::string const &project_name,
                      std::filesystem::path const &project_directory,
                      SceneProto::StructureType structure_type, AmbientComponent *ambient_comp,
                      CameraComponent *camera_comp,
                      EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                      ModificationMonitorComponent *modification_monitor_comp,
                      RendererComponent *renderer_comp, SceneViewComponent *scene_view_comp,
                      EditorContext *context) {
    if (context->game != nullptr) {
        BOOST_LOG_TRIVIAL(error)
            << "CreateNewProject(): A non-null game is in the current editor context.";
        return false;
    }

    context->game = std::make_unique<Game>(project_directory, structure_type, project_name);
    context->game_thread = std::make_unique<std::thread>(RunEditorGame, context->game.get(),
                                                         context->editor_storyline.get());

    editor_portal_switcher_comp->SetEditorPortalEnabled(/*enabled=*/true);
    ambient_comp->OnChangeProject();
    camera_comp->OnChangeProject();
    modification_monitor_comp->OnModifyProject();
    renderer_comp->OnChangeProject();
    scene_view_comp->OnChangeProject();

    return true;
}

} // namespace

namespace project_creator_internal {

ProjectCreationDialog::ProjectCreationDialog() : scene_structure(SceneProto::INVALID) {
    ui.setupUi(this);
    this->setStyleSheet(kQssStyleSheet);

    QObject::connect(ui.project_directory_browse_button, &QPushButton::clicked, this,
                     &ProjectCreationDialog::OnClickBrowseButton);

    QObject::connect(this, &QDialog::accepted, this, &ProjectCreationDialog::OnAcceptInputs);

    ui.project_name_edit->setText(kDefaultSceneName);
}

ProjectCreationDialog::~ProjectCreationDialog() {}

bool ProjectCreationDialog::Valid() const {
    if (project_name.empty()) {
        return false;
    }

    if (!std::filesystem::is_directory(project_directory)) {
        return false;
    }

    if (scene_structure == SceneProto::INVALID) {
        return false;
    }

    return true;
}

void ProjectCreationDialog::OnClickBrowseButton() {
    QString project_directory = QFileDialog::getExistingDirectory(
        /*parent=*/nullptr, /*caption=*/tr("Open Project"), QDir::homePath());
    ui.project_directory_edit->setText(project_directory);
}

void ProjectCreationDialog::OnAcceptInputs() {
    project_name = ui.project_name_edit->text().toStdString();
    project_directory = ui.project_directory_edit->text().toStdString();
    if (ui.linear_structure_button->isChecked()) {
        scene_structure = SceneProto::LINEAR;
    } else if (ui.octree_structure_button->isChecked()) {
        scene_structure = SceneProto::OCTREE;
    }
}

} // namespace project_creator_internal

ProjectCreatorComponent::ProjectCreatorComponent(
    AmbientComponent *ambient_comp, CameraComponent *camera_comp,
    EditorPortalSwitcherComponent *editor_portal_switcher_comp,
    ModificationMonitorComponent *modification_monitor_comp, RendererComponent *renderer_comp,
    SceneViewComponent *scene_view_comp, EditorContext *context)
    : project_creation_dialog_(std::make_unique<project_creator_internal::ProjectCreationDialog>()),
      ambient_comp_(ambient_comp), camera_comp_(camera_comp),
      editor_portal_switcher_comp_(editor_portal_switcher_comp),
      modification_monitor_comp_(modification_monitor_comp), renderer_comp_(renderer_comp),
      scene_view_comp_(scene_view_comp), context_(context) {
    QAction::connect(context_->ui->action_new_project, &QAction::triggered, this,
                     &ProjectCreatorComponent::OnClickNewProject);
}

ProjectCreatorComponent::~ProjectCreatorComponent() {}

void ProjectCreatorComponent::OnClickNewProject() {
    if (context_->game != nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnClickNewProject(): Invoked with an active project.";
        return;
    }

    project_creation_dialog_->exec();

    if (!project_creation_dialog_->Valid()) {
        QMessageBox msg_box;
        msg_box.setStyleSheet(kQssStyleSheet);
        msg_box.setText("Failed to Create Project");
        msg_box.setInformativeText("Invalid Inputs");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();

        return;
    }

    if (!CreateNewProject(project_creation_dialog_->project_name,
                          project_creation_dialog_->project_directory,
                          project_creation_dialog_->scene_structure, ambient_comp_, camera_comp_,
                          editor_portal_switcher_comp_, modification_monitor_comp_, renderer_comp_,
                          scene_view_comp_, context_)) {
        QMessageBox msg_box;
        msg_box.setStyleSheet(kQssStyleSheet);
        msg_box.setText("Failed to Create Project");
        msg_box.setInformativeText("Internal Error");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
}

} // namespace e8
