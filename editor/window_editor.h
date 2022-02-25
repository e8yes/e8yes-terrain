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

#ifndef ISLANDS_EDITOR_WINDOW_H
#define ISLANDS_EDITOR_WINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QWidget>
#include <memory>

#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/component_status.h"
#include "editor/basic/context.h"
#include "editor/environment/component_environment.h"
#include "editor/scene/component_scene_closer.h"
#include "editor/scene/component_scene_loader.h"
#include "editor/scene/component_scene_saver.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {

/**
 * @brief The IslandsEditorWindow class The editor panel.
 */
class IslandsEditorWindow : public QMainWindow {
    Q_OBJECT

  public:
    IslandsEditorWindow(std::shared_ptr<EditorContext> const &editor_context,
                        QWidget *parent = nullptr);
    ~IslandsEditorWindow();

    void closeEvent(QCloseEvent *) override;

  private:
    std::shared_ptr<EditorContext> editor_context_;
    std::unique_ptr<StatusComponent> status_comp_;
    std::unique_ptr<ModificationMonitorComponent> modification_monitor_comp_;
    std::unique_ptr<EditorPortalSwitcherComponent> editor_portal_switcher_comp_;
    std::unique_ptr<EnvironmentComponent> environment_comp_;
    std::unique_ptr<SceneViewComponent> scene_view_comp_;
    std::unique_ptr<SceneSaverComponent> scene_saver_comp_;
    std::unique_ptr<SceneCloserComponent> scene_closer_comp_;
    std::unique_ptr<SceneLoaderComponent> scene_loader_comp_;
};

/**
 * @brief RunIslandsEditorWindow Runs the editor window. This function blocks until the editor
 * window is closed.
 */
void RunIslandsEditorWindow(std::shared_ptr<EditorContext> editor_context, int argc, char *argv[]);

} // namespace e8

#endif // ISLANDS_EDITOR_WINDOW_H
