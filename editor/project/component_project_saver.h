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

#ifndef ISLANDS_EDITOR_COMPONENT_PROJECT_SAVER_H
#define ISLANDS_EDITOR_COMPONENT_PROJECT_SAVER_H

#include <QObject>
#include <optional>
#include <string>

#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {

/**
 * @brief The ProjectSaverComponent class It's responsible for the scene saving interactions.
 */
class ProjectSaverComponent : public QObject {
    Q_OBJECT

  public:
    ProjectSaverComponent(ModificationMonitorComponent *modification_monitor_comp,
                          SceneViewComponent *scene_view_comp, EditorContext *context);
    ~ProjectSaverComponent();

  public slots:
    void OnClickSaveProject();

  private:
    ModificationMonitorComponent *modification_monitor_comp_;
    SceneViewComponent *scene_view_comp_;
    EditorContext *context_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_PROJECT_SAVER_H
