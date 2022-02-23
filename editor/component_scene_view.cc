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

#include <QObject>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "editor/component_editor_context.h"
#include "editor/component_scene_view.h"

namespace e8 {
namespace {

void UpdateSceneView(SceneInterface const *scene, QTreeWidget *scene_view_tree_widget) {
    scene_view_tree_widget->clear();

    if (scene == nullptr) {
        return;
    }

    QTreeWidgetItem *scene_root = new QTreeWidgetItem();
    scene_root->setText(/*column=*/0, scene->name.c_str());
    scene_root->setText(/*column=*/1, scene->id.c_str());

    scene_view_tree_widget->addTopLevelItem(scene_root);
}

} // namespace

SceneViewComponent::SceneViewComponent(EditorContext *context) : context_(context) {
    context_->ui->scene_view_tree_widget->setHeaderLabels(QStringList{"Name", "ID"});
}

SceneViewComponent::~SceneViewComponent() {}

void SceneViewComponent::OnChangeScene() {
    UpdateSceneView(context_->scene.get(), context_->ui->scene_view_tree_widget);
}

} // namespace e8
