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

#ifndef ISLANDS_EDITOR_COMPONENT_SCENE_LOADER_H
#define ISLANDS_EDITOR_COMPONENT_SCENE_LOADER_H

#include <QObject>

#include "editor/basic/component_editor_portal_switcher.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/environment/component_ambient.h"
#include "editor/scene/component_scene_saver.h"
#include "editor/scene/component_scene_view.h"

namespace e8 {

/**
 * @brief The SceneLoaderComponent class It's responsible for scene loading interactions.
 */
class SceneLoaderComponent : public QObject {
    Q_OBJECT

  public:
    SceneLoaderComponent(AmbientComponent *ambient_comp,
                         EditorPortalSwitcherComponent *editor_portal_switcher_comp,
                         ModificationMonitorComponent *modification_monitor_comp,
                         SceneSaverComponent *scene_saver_comp, SceneViewComponent *scene_view_comp,
                         EditorContext *context);
    ~SceneLoaderComponent();

  public slots:
    void OnClickNewSceneLinear();
    void OnClickNewSceneOctree();
    void OnClickOpenScene();

  private:
    AmbientComponent *ambient_comp_;
    EditorPortalSwitcherComponent *editor_portal_switcher_comp_;
    ModificationMonitorComponent *modification_monitor_comp_;
    SceneSaverComponent *scene_saver_comp_;
    SceneViewComponent *scene_view_comp_;

    EditorContext *context_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_SCENE_LOADER_H
