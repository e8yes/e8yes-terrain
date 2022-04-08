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

#include <QGridLayout>
#include <QGroupBox>
#include <QObject>
#include <QRadioButton>
#include <QSlider>
#include <QTextEdit>
#include <QWidget>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <memory>
#include <sstream>
#include <vector>

#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/basic/editor_storyline.h"
#include "editor/display/component_renderer.h"
#include "game/game.h"
#include "game/task.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "ui_renderer_depth_parameters.h"
#include "ui_renderer_radiance_parameters.h"
#include "ui_renderer_radiosity_parameters.h"
#include "ui_renderer_solid_color_parameters.h"

namespace e8 {
namespace {

void DisplayRendererType(RendererConfiguration const &config, QRadioButton *solid_color_rend,
                         QRadioButton *depth_rend, QRadioButton *light_inputs_rend,
                         QRadioButton *radiance_rend, QRadioButton *radiosity_rend) {
    switch (config.in_use_renderer_type()) {
    case RendererType::RT_SOLID_COLOR: {
        solid_color_rend->setChecked(true);
        break;
    }
    case RendererType::RT_DEPTH: {
        depth_rend->setChecked(true);
        break;
    }
    case RendererType::RT_LIGHT_INPUTS: {
        light_inputs_rend->setChecked(true);
        break;
    }
    case RendererType::RT_RADIANCE: {
        radiance_rend->setChecked(true);
        break;
    }
    case RendererType::RT_RADIOSITY: {
        radiosity_rend->setChecked(true);
        break;
    }
    default: {
        assert(false);
    }
    }
}

void DisplayRendererParameters(RendererConfiguration const &config,
                               ModificationMonitorComponent *modification_monitor_comp,
                               EditorContext *context, QGroupBox *target_group_box) {
    auto layout = static_cast<QGridLayout *>(target_group_box->layout());

    QWidget *renderer_params_widget = nullptr;

    switch (config.in_use_renderer_type()) {
    case RendererType::RT_SOLID_COLOR: {
        renderer_params_widget =
            new display_internal::SolidColorRendererWidget(modification_monitor_comp, context);
        break;
    }
    case RendererType::RT_DEPTH: {
        renderer_params_widget =
            new display_internal::DepthRendererWidget(modification_monitor_comp, context);
        break;
    }
    case RendererType::RT_LIGHT_INPUTS: {
        renderer_params_widget =
            new display_internal::LightInputsRendererWidget(modification_monitor_comp, context);
        break;
    }
    case RendererType::RT_RADIANCE: {
        renderer_params_widget =
            new display_internal::RadianceRendererParameters(modification_monitor_comp, context);
        break;
    }
    case RendererType::RT_RADIOSITY: {
        renderer_params_widget =
            new display_internal::RadiosityRendererParameters(modification_monitor_comp, context);
        break;
    }
    default: {
        assert(false);
    }
    }

    layout->addWidget(renderer_params_widget, /*row=*/0, /*column=*/0);
}

void DisplayPerformanceStats(std::vector<RendererInterface::StagePerformance> const &performance,
                             QTextEdit *display_performance_edit) {
    std::stringstream stream;

    for (auto const &stage : performance) {
        stream << "Stage=[" << stage.name << "]" << std::endl;
        stream << "last_1_frame_ms=[" << stage.last_1_frame_ms << "]" << std::endl;
        stream << "last_10_frame_ms=[" << stage.last_10_frame_ms << "]" << std::endl;
        stream << "last_100_frame_ms=[" << stage.last_100_frame_ms << "]" << std::endl;
        stream << "last_1000_frame_ms=[" << stage.last_1000_frame_ms << "]" << std::endl;
        stream << "-----------------------------------" << std::endl;
    }

    std::string stats_string = stream.str();
    display_performance_edit->setText(stats_string.c_str());
}

} // namespace

namespace display_internal {

SolidColorRendererWidget::SolidColorRendererWidget(
    ModificationMonitorComponent *modification_monitor_comp, EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    ui_.setupUi(this);
}

SolidColorRendererWidget::~SolidColorRendererWidget() {}

DepthRendererWidget::DepthRendererWidget(ModificationMonitorComponent *modification_monitor_comp,
                                         EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    ui_.setupUi(this);
    QObject::connect(ui_.alpha_slider, &QSlider::valueChanged, this,
                     &DepthRendererWidget::OnChangeAlpha);

    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    float alpha = config->depth_renderer_params().alpha();
    ui_.alpha_slider->setValue(alpha * ui_.alpha_slider->maximum());
}

DepthRendererWidget::~DepthRendererWidget() {}

void DepthRendererWidget::OnChangeAlpha() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    float alpha = static_cast<float>(ui_.alpha_slider->value()) / ui_.alpha_slider->maximum();
    config->mutable_depth_renderer_params()->set_alpha(alpha);

    modification_monitor_comp_->OnModifyProject();
}

LightInputsRendererWidget::LightInputsRendererWidget(
    ModificationMonitorComponent *modification_monitor_comp, EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    ui_.setupUi(this);

    QObject::connect(ui_.albedo_color_radio, &QRadioButton::clicked, this,
                     &LightInputsRendererWidget::OnChangeInputType);
    QObject::connect(ui_.normal_vector_radio, &QRadioButton::clicked, this,
                     &LightInputsRendererWidget::OnChangeInputType);
    QObject::connect(ui_.roughness_factor_radio, &QRadioButton::clicked, this,
                     &LightInputsRendererWidget::OnChangeInputType);
    QObject::connect(ui_.metallic_factor_radio, &QRadioButton::clicked, this,
                     &LightInputsRendererWidget::OnChangeInputType);

    RendererConfiguration *config = context_->game->GetGameData().renderer_config;

    switch (config->light_inputs_renderer_params().input_to_visualize()) {
    case LightInputsRendererParameters::ALBEDO: {
        ui_.albedo_color_radio->setChecked(true);
        break;
    }
    case LightInputsRendererParameters::NORMAL: {
        ui_.normal_vector_radio->setChecked(true);
        break;
    }
    case LightInputsRendererParameters::ROUGHNESS: {
        ui_.roughness_factor_radio->setChecked(true);
        break;
    }
    case LightInputsRendererParameters::METALLIC: {
        ui_.metallic_factor_radio->setChecked(true);
        break;
    }
    default: {
        assert(false);
    }
    }
}

LightInputsRendererWidget::~LightInputsRendererWidget() {}

void LightInputsRendererWidget::OnChangeInputType() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;

    if (ui_.albedo_color_radio->isChecked()) {
        config->mutable_light_inputs_renderer_params()->set_input_to_visualize(
            LightInputsRendererParameters::ALBEDO);
    } else if (ui_.normal_vector_radio->isChecked()) {
        config->mutable_light_inputs_renderer_params()->set_input_to_visualize(
            LightInputsRendererParameters::NORMAL);
    } else if (ui_.roughness_factor_radio->isChecked()) {
        config->mutable_light_inputs_renderer_params()->set_input_to_visualize(
            LightInputsRendererParameters::ROUGHNESS);
    } else if (ui_.metallic_factor_radio->isChecked()) {
        config->mutable_light_inputs_renderer_params()->set_input_to_visualize(
            LightInputsRendererParameters::METALLIC);
    } else {
        assert(false);
    }

    modification_monitor_comp_->OnModifyProject();
}

RadianceRendererParameters::RadianceRendererParameters(
    ModificationMonitorComponent *modification_monitor_comp, EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    ui_.setupUi(this);
}

RadianceRendererParameters::~RadianceRendererParameters() {}

RadiosityRendererParameters::RadiosityRendererParameters(
    ModificationMonitorComponent *modification_monitor_comp, EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    ui_.setupUi(this);
}

RadiosityRendererParameters::~RadiosityRendererParameters() {}

} // namespace display_internal

RendererComponent::RendererComponent(ModificationMonitorComponent *modification_monitor_comp,
                                     EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    context_->ui->display_performance_edit->setText(
        "No performance statisitcs is available at the moment.");

    QObject::connect(context->ui->solid_color_rend_radial, &QRadioButton::clicked, this,
                     &RendererComponent::OnClickSolidColorRenderer);
    QObject::connect(context->ui->depth_rend_radial, &QRadioButton::clicked, this,
                     &RendererComponent::OnClickDepthRenderer);
    QObject::connect(context->ui->light_inputs_rend_radial, &QRadioButton::clicked, this,
                     &RendererComponent::OnClickLightInputsRenderer);
    QObject::connect(context->ui->radiance_rend_radial, &QRadioButton::clicked, this,
                     &RendererComponent::OnClickRadianceRenderer);
    QObject::connect(context->ui->radiosity_rend_radial, &QRadioButton::clicked, this,
                     &RendererComponent::OnClickRadiosityRenderer);

    auto story_comp = static_cast<EditorStoryComponent *>(
        context_->editor_storyline->FindComponent(kEditorComponent));
    RendererPerformanceUpdateTask *perf_update_task =
        story_comp->GetRendererPerformanceUpdateTask();

    QObject::connect(&perf_update_task->transmitter,
                     &RendererPerformanceUpdateEventTransmitter::UpdateRequired, this,
                     &RendererComponent::OnUpdateRendererPerformanceStatistics);
}

RendererComponent::~RendererComponent() {}

void RendererComponent::OnChangeProject() {
    if (context_->game == nullptr) {
        BOOST_LOG_TRIVIAL(error)
            << "RendererComponent::OnChangeProject(): Invoked with an empty project.";
        return;
    }

    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    DisplayRendererType(*config, context_->ui->solid_color_rend_radial,
                        context_->ui->depth_rend_radial, context_->ui->light_inputs_rend_radial,
                        context_->ui->radiance_rend_radial, context_->ui->radiosity_rend_radial);
    DisplayRendererParameters(*config, modification_monitor_comp_, context_,
                              context_->ui->display_param_group);
}

void RendererComponent::OnClickSolidColorRenderer() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    config->set_in_use_renderer_type(RendererType::RT_SOLID_COLOR);

    DisplayRendererParameters(*config, modification_monitor_comp_, context_,
                              context_->ui->display_param_group);

    modification_monitor_comp_->OnModifyProject();
}

void RendererComponent::OnClickDepthRenderer() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    config->set_in_use_renderer_type(RendererType::RT_DEPTH);

    DisplayRendererParameters(*config, modification_monitor_comp_, context_,
                              context_->ui->display_param_group);

    modification_monitor_comp_->OnModifyProject();
}

void RendererComponent::OnClickLightInputsRenderer() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    config->set_in_use_renderer_type(RendererType::RT_LIGHT_INPUTS);

    DisplayRendererParameters(*config, modification_monitor_comp_, context_,
                              context_->ui->display_param_group);

    modification_monitor_comp_->OnModifyProject();
}

void RendererComponent::OnClickRadianceRenderer() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    config->set_in_use_renderer_type(RendererType::RT_RADIANCE);

    DisplayRendererParameters(*config, modification_monitor_comp_, context_,
                              context_->ui->display_param_group);

    modification_monitor_comp_->OnModifyProject();
}

void RendererComponent::OnClickRadiosityRenderer() {
    RendererConfiguration *config = context_->game->GetGameData().renderer_config;
    config->set_in_use_renderer_type(RendererType::RT_RADIOSITY);

    DisplayRendererParameters(*config, modification_monitor_comp_, context_,
                              context_->ui->display_param_group);

    modification_monitor_comp_->OnModifyProject();
}

void RendererComponent::OnUpdateRendererPerformanceStatistics() {
    RendererInterface *renderer = context_->game->GetGameData().renderer;
    std::vector<RendererInterface::StagePerformance> performance = renderer->GetPerformanceStats();

    DisplayPerformanceStats(performance, context_->ui->display_performance_edit);
}

} // namespace e8
