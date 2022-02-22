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
#include <QObject>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <boost/log/trivial.hpp>
#include <memory>
#include <vector>

#include "content/scene_linear.h"
#include "editor/component_editor_context.h"
#include "editor/component_scene.h"
#include "ui_window_editor.h"

namespace e8 {
namespace {

enum EditorSceneType {
    EST_INVALID,
    EST_LINEAR_SCENE,
    EST_OCTREE_SCENE,
};

constexpr char const *kDefaultSceneName = "Untitled";

void SetSceneCreationWidgetsEnabled(bool enabled, Ui::IslandsEditorWindow *ui) {
    std::vector<QAction *> actions{
        ui->action_new_scene_flat,
        ui->action_new_scene_octree,
        ui->action_open_scene,
    };
    for (auto action : actions) {
        action->setEnabled(enabled);
    }

    std::vector<QWidget *> widgets{ui->menu_new_scene};
    for (auto widget : widgets) {
        widget->setEnabled(enabled);
    }
}

void SetEditorWidgetsEnabled(bool enabled, Ui::IslandsEditorWindow *ui) {
    std::vector<QAction *> actions{
        ui->action_close_scene,
        ui->action_save_scene,
    };
    for (auto action : actions) {
        action->setEnabled(enabled);
    }

    std::vector<QWidget *> widgets{
        ui->menu_add_scene_object,
    };
    DeepScanWidget(ui->design_nav_tabs, &widgets);
    DeepScanWidget(ui->image_input_graphics_view, &widgets);
    for (auto widget : widgets) {
        widget->setEnabled(enabled);
    }
}

void InitSceneView(SceneInterface const *scene, QTreeWidget *scene_view_tree_widget) {
    QTreeWidgetItem *scene_root = new QTreeWidgetItem();
    scene_root->setText(/*column=*/0, scene->name.c_str());
    scene_root->setText(/*column=*/1, scene->id.c_str());

    scene_view_tree_widget->addTopLevelItem(scene_root);
}

bool CreateNewScene(EditorSceneType scene_type, EditorContext *context) {
    if (context->scene != nullptr) {
        BOOST_LOG_TRIVIAL(error)
            << "CreateNewScene(): A non-null scene object is in the current editor context.";
        return false;
    }

    switch (scene_type) {
    case EST_LINEAR_SCENE: {
        context->scene = std::make_unique<LinearScene>(kDefaultSceneName);
        break;
    }
    case EST_OCTREE_SCENE: {
        // TODO: Creates an octree scene when it is implemented.
        BOOST_LOG_TRIVIAL(error) << "CreateNewScene(): Octree scene is not implemented.";
        return false;
    }
    default: {
        BOOST_LOG_TRIVIAL(error) << "CreateNewScene(): Unknown scene type.";
        return false;
    }
    }

    SetSceneCreationWidgetsEnabled(/*enabled=*/false, context->ui.get());
    SetEditorWidgetsEnabled(/*enabled=*/true, context->ui.get());
    InitSceneView(context->scene.get(), context->ui->scene_view_tree_widget);

    return true;
}

} // namespace

SceneComponent::SceneComponent(EditorContext *context) : context_(context) {
    context_->ui->scene_view_tree_widget->setHeaderLabels(QStringList{"Name", "ID"});

    QAction::connect(context_->ui->action_new_scene_flat, &QAction::triggered, this,
                     &SceneComponent::OnClickNewSceneLinear);
    QAction::connect(context_->ui->action_new_scene_octree, &QAction::triggered, this,
                     &SceneComponent::OnClickNewSceneOctree);
}

SceneComponent::~SceneComponent() {}

void SceneComponent::OnClickNewSceneLinear() {
    CreateNewScene(/*scene_type=*/EST_LINEAR_SCENE, context_);
}

void SceneComponent::OnClickNewSceneOctree() {
    CreateNewScene(/*scene_type=*/EST_OCTREE_SCENE, context_);
}

} // namespace e8
