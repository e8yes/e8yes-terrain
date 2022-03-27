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
#include <algorithm>
#include <memory>

#include "content/procedural_basic.h"
#include "content/scene.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/scene/component_scene_view.h"
#include "procedural/component_procedural_plane.h"

namespace e8 {
namespace {

constexpr char const *kPlaneObjectName = "plane";

} // namespace

ProceduralPlaneComponent::ProceduralPlaneComponent(
    ModificationMonitorComponent *modification_monitor_comp, SceneViewComponent *scene_view_comp,
    EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), scene_view_comp_(scene_view_comp),
      context_(context) {
    QAction::connect(context->ui->action_add_plane, &QAction::triggered, this,
                     &ProceduralPlaneComponent::OnClickAddProceduralPlane);
}

ProceduralPlaneComponent::~ProceduralPlaneComponent() {}

void ProceduralPlaneComponent::OnClickAddProceduralPlane() {
    {
        GameData game_data = context_->game->GetGameData();

        Scene::WriteAccess write_access = game_data.scene->GainWriteAccess();

        auto plane = std::make_unique<ProceduralPlane>(kPlaneObjectName, /*width=*/1, /*height=*/1,
                                                       /*cell_area=*/1);
        game_data.scene->AddProceduralObject(std::move(plane), game_data.resource_accessor);
    }

    modification_monitor_comp_->OnModifyProject();
    scene_view_comp_->OnChangeProject();
}

} // namespace e8
