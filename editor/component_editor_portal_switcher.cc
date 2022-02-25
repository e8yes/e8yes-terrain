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
#include <QWidget>
#include <vector>

#include "editor/component_editor_portal_switcher.h"
#include "editor/context.h"
#include "ui_window_editor.h"

namespace e8 {
namespace {

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

} // namespace

EditorPortalSwitcherComponent::EditorPortalSwitcherComponent(EditorContext *context)
    : context_(context) {}

EditorPortalSwitcherComponent::~EditorPortalSwitcherComponent() {}

void EditorPortalSwitcherComponent::SetEditorPortalEnabled(bool enabled) {
    SetSceneCreationWidgetsEnabled(/*enabled=*/!enabled, context_->ui.get());
    SetEditorWidgetsEnabled(/*enabled=*/enabled, context_->ui.get());
}

} // namespace e8
