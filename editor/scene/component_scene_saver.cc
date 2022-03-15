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
#include <QFileInfo>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <boost/log/trivial.hpp>
#include <fstream>
#include <optional>
#include <string>

#include "content/proto/scene.pb.h"
#include "content/scene.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/scene/component_scene_saver.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {
namespace {

constexpr char const *kSceneFileSuffix = "pb";

QString FixSceneFilePath(QString const target_file_path) {
    QFileInfo file_info(target_file_path);
    if (file_info.suffix() != kSceneFileSuffix) {
        return target_file_path + "." + kSceneFileSuffix;
    }
    return target_file_path;
}

void RenameSceneToFileName(QString const &target_file_path, Scene *scene,
                           SceneViewComponent *scene_view_comp) {
    QFileInfo file_info(target_file_path);
    scene->name = file_info.baseName().toStdString();
    scene_view_comp->OnChangeScene();
}

bool SaveScene(Scene const &scene, std::string const &target_file_path) {
    std::fstream file(target_file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "SaveScene(): Failed to open scene file=[" << target_file_path
                                 << "].";
        return false;
    }

    SceneProto proto = scene.ToProto();
    if (!proto.SerializeToOstream(&file)) {
        BOOST_LOG_TRIVIAL(error) << "SaveScene(): Scene serialization error.";
        return false;
    }

    return true;
}

} // namespace

SceneSaverComponent::SceneSaverComponent(ModificationMonitorComponent *modification_monitor_comp,
                                         SceneViewComponent *scene_view_comp,
                                         EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), scene_view_comp_(scene_view_comp),
      context_(context), unsaved_modifications_(false) {
    QAction::connect(context_->ui->action_save_scene, &QAction::triggered, this,
                     &SceneSaverComponent::OnClickSaveScene);
}

SceneSaverComponent::~SceneSaverComponent() {}

void SceneSaverComponent::OnChangeLoadPath(std::optional<std::string> const &load_path) {
    load_path_ = load_path;
}

void SceneSaverComponent::OnClickSaveScene() {
    if (context_->scene == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "SceneSaverComponent(): Nothing to save.";
        return;
    }

    if (!modification_monitor_comp_->UnsavedModifications()) {
        BOOST_LOG_TRIVIAL(error) << "SceneSaverComponent(): Invoked without unsaved modifications.";
        return;
    }

    QString save_path;
    if (!load_path_.has_value()) {
        save_path = QFileDialog::getSaveFileName(
            /*parent=*/nullptr, /*capture=*/tr("Save Scene"), QDir::homePath(),
            /*filter=*/tr("e8 islands scene (*.pb)"));
        save_path = FixSceneFilePath(save_path);
    } else {
        save_path = QString::fromStdString(*load_path_);
    }

    RenameSceneToFileName(save_path, context_->scene.get(), scene_view_comp_);

    if (!SaveScene(*context_->scene, save_path.toStdString())) {
        QMessageBox msg_box;
        msg_box.setText("Failed to Save Scene");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();

        return;
    }

    load_path_ = save_path.toStdString();

    modification_monitor_comp_->OnReset();
}

} // namespace e8
