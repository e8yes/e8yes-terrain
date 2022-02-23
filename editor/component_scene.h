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

#ifndef ISLANDS_EDITOR_COMPONENT_SCENE_H
#define ISLANDS_EDITOR_COMPONENT_SCENE_H

#include <QObject>

#include "editor/component_editor_context.h"
#include "editor/component_environment.h"

namespace e8 {

/**
 * @brief The SceneComponent class Handles logic with UIs related to scene management.
 */
class SceneComponent : public QObject {
    Q_OBJECT

  public:
    SceneComponent(EnvironmentComponent *environment_component, EditorContext *context);
    ~SceneComponent();

  public slots:
    void OnClickNewSceneLinear();

    void OnClickNewSceneOctree();

    void OnClickOpenScene();

    void OnClickSaveScene();

    void OnClickCloseScene();

  private:
    EnvironmentComponent *environment_component_;
    EditorContext *context_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_SCENE_H
