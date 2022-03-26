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

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <thread>

#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/environment/component_ambient.h"
#include "editor/environment/component_camera.h"
#include "editor/project/component_project_loader.h"
#include "editor/scene/component_scene_view.h"
#include "game/game.h"

namespace e8 {
namespace {

constexpr char const *kDefaultSceneName = "Untitled";

bool CreateNewProject(SceneProto::StructureType structure_type, AmbientComponent *ambient_comp,
                      CameraComponent *camera_comp,
                      EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                      ModificationMonitorComponent *modification_monitor_comp,
                      SceneViewComponent *scene_view_comp, EditorContext *context) {
    if (context->game != nullptr) {
        BOOST_LOG_TRIVIAL(error)
            << "CreateNewScene(): A non-null game is in the current editor context.";
        return false;
    }

    context->game = std::make_unique<Game>("/home/davis/sponza", structure_type, kDefaultSceneName);
    context->game_thread = std::make_unique<std::thread>(RunEditorGame, context->game.get(),
                                                         context->editor_storyline.get());

    editor_portal_switcher_comp->SetEditorPortalEnabled(/*enabled=*/true);
    ambient_comp->OnChangeScene();
    camera_comp->OnChangeScene();
    modification_monitor_comp->OnModifyScene();
    scene_view_comp->OnChangeScene();

    return true;
}

bool LoadProject(std::filesystem::path const &game_path, AmbientComponent *ambient_comp,
                 CameraComponent *camera_comp,
                 EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                 ModificationMonitorComponent *modification_monitor_comp,
                 SceneViewComponent *scene_view_comp, EditorContext *context) {
    context->game = std::make_unique<Game>(game_path);
    if (!context->game->Valid()) {
        context->game = nullptr;
        return false;
    }

    context->game_thread = std::make_unique<std::thread>(RunEditorGame, context->game.get(),
                                                         context->editor_storyline.get());

    editor_portal_switcher_comp->SetEditorPortalEnabled(/*enabled=*/true);
    ambient_comp->OnChangeScene();
    camera_comp->OnChangeScene();
    modification_monitor_comp->OnReset();
    scene_view_comp->OnChangeScene();

    return true;
}

} // namespace

ProjectLoaderComponent::ProjectLoaderComponent(
    AmbientComponent *ambient_comp, CameraComponent *camera_comp,
    EditorPortalSwitcherComponent *editor_portal_switcher_comp,
    ModificationMonitorComponent *modification_monitor_comp, SceneViewComponent *scene_view_comp,
    EditorContext *context)
    : ambient_comp_(ambient_comp), camera_comp_(camera_comp),
      editor_portal_switcher_comp_(editor_portal_switcher_comp),
      modification_monitor_comp_(modification_monitor_comp), scene_view_comp_(scene_view_comp),
      context_(context) {
    QAction::connect(context_->ui->action_new_scene_flat, &QAction::triggered, this,
                     &ProjectLoaderComponent::OnClickNewProject);
    QAction::connect(context_->ui->action_open_scene, &QAction::triggered, this,
                     &ProjectLoaderComponent::OnClickOpenProject);
}

ProjectLoaderComponent::~ProjectLoaderComponent() {}

void ProjectLoaderComponent::OnClickNewProject() {
    if (context_->game != nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnClickNewSceneLinear(): Invoked with an active scene.";
        return;
    }

    if (!CreateNewProject(/*scene_type=*/SceneProto::LINEAR, ambient_comp_, camera_comp_,
                          editor_portal_switcher_comp_, modification_monitor_comp_,
                          scene_view_comp_, context_)) {
        QMessageBox msg_box;
        msg_box.setText("Failed to Create Scene");
        msg_box.setInformativeText("Internal Error");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
}

void ProjectLoaderComponent::OnClickOpenProject() {
    if (context_->game != nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnClickOpenScene(): Invoked with an active scene.";
        return;
    }

    QString base_path = QFileDialog::getExistingDirectory(
        /*parent=*/nullptr, /*caption=*/tr("Open Game"), QDir::homePath());

    if (!LoadProject(base_path.toStdString(), ambient_comp_, camera_comp_,
                     editor_portal_switcher_comp_, modification_monitor_comp_, scene_view_comp_,
                     context_)) {
        QMessageBox msg_box;
        msg_box.setText("Failed to Load Scene");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
}

} // namespace e8
