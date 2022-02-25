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
#include <fstream>
#include <memory>

#include "content/structure_linear.h"
#include "content/proto/scene.pb.h"
#include "content/scene.h"
#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/environment/component_environment.h"
#include "editor/scene/component_scene_loader.h"
#include "editor/scene/component_scene_saver.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {
namespace {

constexpr char const *kDefaultSceneName = "Untitled";

bool CreateNewScene(SceneProto::StructureType structure_type,
                    EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                    EnvironmentComponent *environment_comp,
                    ModificationMonitorComponent *modification_monitor_comp,
                    SceneViewComponent *scene_view_comp, EditorContext *context) {
    if (context->scene != nullptr) {
        BOOST_LOG_TRIVIAL(error)
            << "CreateNewScene(): A non-null scene object is in the current editor context.";
        return false;
    }

    context->scene = std::make_unique<Scene>(structure_type, kDefaultSceneName);

    editor_portal_switcher_comp->SetEditorPortalEnabled(/*enabled=*/true);
    environment_comp->OnChangeScene();
    modification_monitor_comp->OnModifyScene();
    scene_view_comp->OnChangeScene();

    return true;
}

bool LoadScene(std::string const &scene_file,
               EditorPortalSwitcherComponent *editor_portal_switcher_comp,
               EnvironmentComponent *environment_comp,
               ModificationMonitorComponent *modification_monitor_comp,
               SceneSaverComponent *scene_saver_comp, SceneViewComponent *scene_view_comp,
               EditorContext *context) {
    std::fstream file(scene_file, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "LoadScene(): Failed to open scene_file=[" << scene_file
                                 << "].";
        return false;
    }

    SceneProto proto;
    if (!proto.ParseFromIstream(&file)) {
        BOOST_LOG_TRIVIAL(error) << "LoadScene(): Failed to parse scene_file=[" << scene_file
                                 << "].";
        return false;
    }

    context->scene = std::make_unique<Scene>(proto);

    editor_portal_switcher_comp->SetEditorPortalEnabled(/*enabled=*/true);
    environment_comp->OnChangeScene();
    modification_monitor_comp->OnReset();
    scene_saver_comp->OnChangeLoadPath(scene_file);
    scene_view_comp->OnChangeScene();

    return true;
}

} // namespace

SceneLoaderComponent::SceneLoaderComponent(
    EditorPortalSwitcherComponent *editor_portal_switcher_comp,
    EnvironmentComponent *environment_comp, ModificationMonitorComponent *modification_monitor_comp,
    SceneSaverComponent *scene_saver_comp, SceneViewComponent *scene_view_comp,
    EditorContext *context)
    : editor_portal_switcher_comp_(editor_portal_switcher_comp),
      environment_comp_(environment_comp), modification_monitor_comp_(modification_monitor_comp),
      scene_saver_comp_(scene_saver_comp), scene_view_comp_(scene_view_comp), context_(context) {
    QAction::connect(context_->ui->action_new_scene_flat, &QAction::triggered, this,
                     &SceneLoaderComponent::OnClickNewSceneLinear);
    QAction::connect(context_->ui->action_new_scene_octree, &QAction::triggered, this,
                     &SceneLoaderComponent::OnClickNewSceneOctree);
    QAction::connect(context_->ui->action_open_scene, &QAction::triggered, this,
                     &SceneLoaderComponent::OnClickOpenScene);
}

SceneLoaderComponent::~SceneLoaderComponent() {}

void SceneLoaderComponent::OnClickNewSceneLinear() {
    if (context_->scene != nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnClickNewSceneLinear(): Invoked with an active scene.";
        return;
    }

    if (!CreateNewScene(/*scene_type=*/SceneProto::LINEAR, editor_portal_switcher_comp_,
                        environment_comp_, modification_monitor_comp_, scene_view_comp_,
                        context_)) {
        QMessageBox msg_box;
        msg_box.setText("Failed to Create Scene");
        msg_box.setInformativeText("Internal Error");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
}

void SceneLoaderComponent::OnClickNewSceneOctree() {
    if (context_->scene != nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnClickNewSceneOctree(): Invoked with an active scene.";
        return;
    }

    if (!CreateNewScene(/*scene_type=*/SceneProto::OCTREE, editor_portal_switcher_comp_,
                        environment_comp_, modification_monitor_comp_, scene_view_comp_,
                        context_)) {
    }
}

void SceneLoaderComponent::OnClickOpenScene() {
    if (context_->scene != nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnClickOpenScene(): Invoked with an active scene.";
        return;
    }

    QString scene_file = QFileDialog::getOpenFileName(
        /*parent=*/nullptr, /*caption=*/tr("Open Scene"), QDir::homePath(),
        /*filter=*/tr("e8 islands scene (*.pb)"));

    if (!LoadScene(scene_file.toStdString(), editor_portal_switcher_comp_, environment_comp_,
                   modification_monitor_comp_, scene_saver_comp_, scene_view_comp_, context_)) {
        QMessageBox msg_box;
        msg_box.setText("Failed to Load Scene");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
}

} // namespace e8
