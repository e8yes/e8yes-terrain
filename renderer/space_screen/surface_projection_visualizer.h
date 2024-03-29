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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_SURFACE_PROJECTION_VISUALIZER_H
#define ISLANDS_RENDERER_SPACE_SCREEN_SURFACE_PROJECTION_VISUALIZER_H

#include <memory>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/proto/renderer.pb.h"

namespace e8 {

/**
 * @brief DoVisualizeSurfaceProjection A screen space processing graphics pipeline for visualizing a
 * rendered surface parameter map.
 *
 * @param parameter_to_visualize Selects the surface parameter to be visualized.
 * @param surface_projection The surface parameter maps to select parameter from.
 * @param dag DAG context.
 * @return The target operation which visualizes the specified lighting parameter (light inputs).
 */
DagOperationInstance DoVisualizeSurfaceProjection(
    LightInputsRendererParameters::InputType parameter_to_visualize,
    DagOperationInstance surface_projection,
    std::shared_ptr<GraphicsPipelineOutputInterface> const &color_image_output,
    DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_SURFACE_PROJECTION_VISUALIZER_H
