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

#ifndef ISLANDS_EDITOR_COMPONENT_RENDERER_H
#define ISLANDS_EDITOR_COMPONENT_RENDERER_H

#include <QObject>
#include <QWidget>
#include <memory>

#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "ui_renderer_depth_parameters.h"
#include "ui_renderer_light_inputs_parameters.h"
#include "ui_renderer_radiance_parameters.h"
#include "ui_renderer_radiosity_parameters.h"
#include "ui_renderer_solid_color_parameters.h"

namespace e8 {
namespace display_internal {

class SolidColorRendererWidget : public QWidget {
    Q_OBJECT

  public:
    SolidColorRendererWidget(ModificationMonitorComponent *modification_monitor_comp,
                             EditorContext *context);
    ~SolidColorRendererWidget();

  private:
    Ui::SolidColorRendererParameters ui_;

    ModificationMonitorComponent *modification_monitor_comp_;
    EditorContext *context_;
};

class DepthRendererWidget : public QWidget {
    Q_OBJECT

  public:
    DepthRendererWidget(ModificationMonitorComponent *modification_monitor_comp,
                        EditorContext *context);
    ~DepthRendererWidget();

  public slots:
    void OnChangeAlpha();

  private:
    Ui::DepthRendererParameters ui_;

    ModificationMonitorComponent *modification_monitor_comp_;
    EditorContext *context_;
};

class LightInputsRendererWidget : public QWidget {
    Q_OBJECT

  public:
    LightInputsRendererWidget(ModificationMonitorComponent *modification_monitor_comp,
                              EditorContext *context);
    ~LightInputsRendererWidget();

  public slots:
    void OnChangeInputType();

  private:
    Ui::LightInputsRendererParameters ui_;

    ModificationMonitorComponent *modification_monitor_comp_;
    EditorContext *context_;
};

class RadianceRendererParameters : public QWidget {
    Q_OBJECT

  public:
    RadianceRendererParameters(ModificationMonitorComponent *modification_monitor_comp,
                               EditorContext *context);
    ~RadianceRendererParameters();

  private:
    Ui::RadianceRendererParameters ui_;

    ModificationMonitorComponent *modification_monitor_comp_;
    EditorContext *context_;
};

class RadiosityRendererParameters : public QWidget {
    Q_OBJECT

  public:
    RadiosityRendererParameters(ModificationMonitorComponent *modification_monitor_comp,
                                EditorContext *context);
    ~RadiosityRendererParameters();

  private:
    Ui::RadiosityRendererParameters ui_;

    ModificationMonitorComponent *modification_monitor_comp_;
    EditorContext *context_;
};

} // namespace display_internal

/**
 * @brief The RendererComponent class Responsible for handling renderer interactions.
 */
class RendererComponent : public QObject {
    Q_OBJECT

  public:
    RendererComponent(ModificationMonitorComponent *modification_monitor_comp,
                      EditorContext *context);
    ~RendererComponent();

  public slots:
    void OnChangeProject();
    void OnClickSolidColorRenderer();
    void OnClickDepthRenderer();
    void OnClickLightInputsRenderer();
    void OnClickRadianceRenderer();
    void OnClickRadiosityRenderer();
    void OnUpdateRendererPerformanceStatistics();

  private:
    ModificationMonitorComponent *modification_monitor_comp_;
    EditorContext *context_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_RENDERER_H
