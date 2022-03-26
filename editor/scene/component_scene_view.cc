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
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <map>
#include <string>

#include "content/procedural_basic.h"
#include "content/scene.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "editor/basic/context.h"
#include "editor/scene/component_scene_view.h"
#include "game/game.h"
#include "resource/common.h"

namespace e8 {
namespace {

std::string ItemName(std::string const &name, Uuid const &id) {
    return name + '@' + std::to_string(id).substr(0, 3);
}

void AddSceneEntity(SceneEntity const &scene_entity, QTreeWidgetItem *parent_node) {
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(/*column=*/0, ItemName(scene_entity.name, scene_entity.id).c_str());
    child->setData(/*column=*/0, /*role=*/Qt::UserRole,
                   QVariant(QString(std::to_string(scene_entity.id).c_str())));
    child->setText(/*column=*/1, kSceneEntityType);
    child->setData(/*column=*/1, /*role=*/Qt::UserRole, QVariant(QString(kSceneEntityType)));

    parent_node->addChild(child);
}

void AddSceneObject(SceneObject const &scene_object, QTreeWidgetItem *parent_node) {
    QTreeWidgetItem *current_node = new QTreeWidgetItem();

    current_node->setText(/*column=*/0, ItemName(scene_object.name, scene_object.id).c_str());
    current_node->setData(/*column=*/0, /*role=*/Qt::UserRole,
                          QVariant(QString(std::to_string(scene_object.id).c_str())));
    if (scene_object.Procedural()) {
        current_node->setText(/*column=*/1, kProceduralSceneObjectType);
        current_node->setData(/*column=*/1, /*role=*/Qt::UserRole,
                              QVariant(QString(kProceduralSceneObjectType)));
    } else {
        current_node->setText(/*column=*/1, kSceneObjectType);
        current_node->setData(/*column=*/1, /*role=*/Qt::UserRole,
                              QVariant(QString(kSceneObjectType)));
    }

    parent_node->addChild(current_node);

    // Handles child elements.
    if (scene_object.HasSceneEntityChildren()) {
        for (auto const &child_entity : scene_object.child_scene_entities) {
            AddSceneEntity(child_entity, current_node);
        }
    } else {
        for (auto const &child_object : scene_object.child_scene_objects) {
            AddSceneObject(child_object, current_node);
        }
    }
}

void UpdateSceneView(Game *game, QTreeWidget *scene_view_tree_widget) {
    scene_view_tree_widget->clear();

    if (game == nullptr) {
        return;
    }

    Scene const *scene = game->GetGameData().scene;

    QTreeWidgetItem *scene_root = new QTreeWidgetItem();
    scene_root->setText(/*column=*/0, ItemName(scene->name, scene->id).c_str());
    scene_root->setData(/*column=*/0, /*role=*/Qt::UserRole,
                        QVariant(QString(std::to_string(scene->id).c_str())));
    scene_root->setText(/*column=*/1, kSceneType);
    scene_root->setData(/*column=*/1, /*role=*/Qt::UserRole, QVariant(QString(kSceneType)));

    for (auto const &[_, scene_object] : scene->AllRootSceneObjects()) {
        AddSceneObject(scene_object, scene_root);
    }

    scene_view_tree_widget->addTopLevelItem(scene_root);
}

} // namespace

SceneViewComponent::SceneViewComponent(EditorContext *context) : context_(context) {
    context_->ui->scene_view_tree_widget->setHeaderLabels(QStringList{"Name", "Type"});
}

SceneViewComponent::~SceneViewComponent() {}

void SceneViewComponent::OnChangeScene() {
    UpdateSceneView(context_->game.get(), context_->ui->scene_view_tree_widget);
}

} // namespace e8
