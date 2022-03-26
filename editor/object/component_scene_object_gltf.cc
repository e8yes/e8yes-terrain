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
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <vector>

#include "content/loader_gltf.h"
#include "content/scene.h"
#include "content/scene_object.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/basic/theme.h"
#include "editor/object/component_scene_object_gltf.h"
#include "editor/scene/component_scene_view.h"
#include "resource/accessor.h"

namespace e8 {
namespace {

bool AddGltfSceneObjects(std::string const &gltf_file_path, Scene *scene,
                         ResourceAccessor *resource_context) {
    std::vector<SceneObject> scene_objects = LoadFromGltf(gltf_file_path, resource_context);
    if (scene_objects.empty()) {
        return false;
    }

    {
        Scene::WriteAccess write_access = scene->GainWriteAccess();

        for (auto const &scene_object : scene_objects) {
            scene->AddRootSceneObject(scene_object);
        }
    }

    return true;
}

} // namespace

SceneObjectGltfComponent::SceneObjectGltfComponent(
    ModificationMonitorComponent *modification_monitor_comp, SceneViewComponent *scene_view_comp,
    EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), scene_view_comp_(scene_view_comp),
      context_(context) {
    QAction::connect(context->ui->action_add_gltf_file, &QAction::triggered, this,
                     &SceneObjectGltfComponent::OnClickAddGltfSceneObject);
}

SceneObjectGltfComponent::~SceneObjectGltfComponent() {}

void SceneObjectGltfComponent::OnClickAddGltfSceneObject() {
    QString gltf_file = QFileDialog::getOpenFileName(
        /*parent=*/nullptr, /*caption=*/tr("Open glTF File"), QDir::homePath(),
        /*filter=*/tr("glTF File (*.glb *.gltf)"));

    if (!AddGltfSceneObjects(gltf_file.toStdString(), context_->game->GetGameData().scene,
                             context_->game->GetGameData().resource_accessor)) {
        QMessageBox msg_box;
        msg_box.setStyleSheet(kQssStyleSheet);
        msg_box.setText("Empty glTF file");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }

    modification_monitor_comp_->OnModifyScene();
    scene_view_comp_->OnChangeScene();
}

} // namespace e8
