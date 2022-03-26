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
#include "editor/project/component_project_saver.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {

ProjectSaverComponent::ProjectSaverComponent(
    ModificationMonitorComponent *modification_monitor_comp, SceneViewComponent *scene_view_comp,
    EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), scene_view_comp_(scene_view_comp),
      context_(context) {
    QAction::connect(context_->ui->action_save_scene, &QAction::triggered, this,
                     &ProjectSaverComponent::OnClickSaveProject);
}

ProjectSaverComponent::~ProjectSaverComponent() {}

void ProjectSaverComponent::OnClickSaveProject() {
    if (context_->game == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "SceneSaverComponent(): Nothing to save.";
        return;
    }

    if (!modification_monitor_comp_->UnsavedModifications()) {
        BOOST_LOG_TRIVIAL(error) << "SceneSaverComponent(): Invoked without unsaved modifications.";
        return;
    }

    if (!context_->game->Save()) {
        QMessageBox msg_box;
        msg_box.setText("Failed to Save Game");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();

        return;
    }

    modification_monitor_comp_->OnReset();
}

} // namespace e8
