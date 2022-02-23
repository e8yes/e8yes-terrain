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
#include <algorithm>

#include "common/tensor.h"
#include "editor/component_editor_context.h"
#include "editor/component_environment.h"
#include "editor/component_modification_monitor.h"

namespace e8 {
namespace {

void SetBackgroundColorSlider(SceneInterface const &scene, QSlider *red, QSlider *green,
                              QSlider *blue) {
    vec3 current_color = scene.BackgroundColor();

    red->setValue(current_color(0) * red->maximum());
    green->setValue(current_color(1) * green->maximum());
    blue->setValue(current_color(2) * blue->maximum());
}

void SetSceneBackgroundColor(QSlider const *red, QSlider const *green, QSlider const *blue,
                             SceneInterface *scene) {
    vec3 new_background_color{static_cast<float>(red->value()) / red->maximum(),
                              static_cast<float>(green->value()) / green->maximum(),
                              static_cast<float>(blue->value()) / blue->maximum()};
    scene->UpdateBackgroundColor(new_background_color);
}

} // namespace

EnvironmentComponent::EnvironmentComponent(ModificationMonitorComponent *modification_monitor_comp,
                                           EditorContext *context)
    : context_(context), modification_monitor_comp_(modification_monitor_comp) {
    QObject::connect(context->ui->bg_color_red_slider, &QSlider::valueChanged, this,
                     &EnvironmentComponent::OnChangeBackgroundColor);
    QObject::connect(context->ui->bg_color_green_slider, &QSlider::valueChanged, this,
                     &EnvironmentComponent::OnChangeBackgroundColor);
    QObject::connect(context->ui->bg_color_blue_slider, &QSlider::valueChanged, this,
                     &EnvironmentComponent::OnChangeBackgroundColor);
}

EnvironmentComponent::~EnvironmentComponent() {}

void EnvironmentComponent::OnChangeScene() {
    SetBackgroundColorSlider(*context_->scene, context_->ui->bg_color_red_slider,
                             context_->ui->bg_color_green_slider,
                             context_->ui->bg_color_blue_slider);
}

void EnvironmentComponent::OnChangeBackgroundColor(int /*value*/) {
    SetSceneBackgroundColor(context_->ui->bg_color_red_slider, context_->ui->bg_color_green_slider,
                            context_->ui->bg_color_blue_slider, context_->scene.get());

    modification_monitor_comp_->OnModifyScene();
}

} // namespace e8
