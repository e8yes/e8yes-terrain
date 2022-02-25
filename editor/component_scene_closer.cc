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
#include <QMessageBox>
#include <QObject>

#include "editor/component_editor_portal_switcher.h"
#include "editor/component_modification_monitor.h"
#include "editor/component_scene_closer.h"
#include "editor/component_scene_saver.h"
#include "editor/component_scene_view.h"
#include "editor/context.h"

namespace e8 {
namespace {

void CloseScene(EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                ModificationMonitorComponent *modification_monitor_comp,
                SceneViewComponent *scene_view_comp, EditorContext *context) {
    context->scene = nullptr;
    editor_portal_switcher_comp->SetEditorPortalEnabled(/*enabled=*/false);
    modification_monitor_comp->OnReset();
    scene_view_comp->OnChangeScene();
}

} // namespace

SceneCloserComponent::SceneCloserComponent(
    EditorPortalSwitcherComponent *editor_portal_switcher_comp,
    ModificationMonitorComponent *modification_monitor_comp, SceneSaverComponent *scene_saver_comp,
    SceneViewComponent *scene_view_comp, EditorContext *context)
    : editor_portal_switcher_comp_(editor_portal_switcher_comp),
      modification_monitor_comp_(modification_monitor_comp), scene_saver_comp_(scene_saver_comp),
      scene_view_comp_(scene_view_comp), context_(context) {
    QAction::connect(context_->ui->action_close_scene, &QAction::triggered, this,
                     &SceneCloserComponent::OnClickCloseScene);
}

SceneCloserComponent::~SceneCloserComponent() {}

void SceneCloserComponent::OnClickCloseScene() {
    if (modification_monitor_comp_->UnsavedModifications()) {
        QMessageBox msg_box;
        msg_box.setText("The scene has unsaved modifications.");
        msg_box.setDetailedText("Do you want to save them?");
        msg_box.setStandardButtons(QMessageBox::Save | QMessageBox::Close);
        msg_box.setDefaultButton(QMessageBox::Save);

        if (QMessageBox::Save == msg_box.exec()) {
            scene_saver_comp_->OnClickSaveScene();
        }
    }

    CloseScene(editor_portal_switcher_comp_, modification_monitor_comp_, scene_view_comp_,
               context_);
}

} // namespace e8
