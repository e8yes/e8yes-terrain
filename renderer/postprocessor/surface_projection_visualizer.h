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

#ifndef ISLANDS_RENDERER_POSTPROCESSOR_SURFACE_PROJECTION_VISUALIZER_H
#define ISLANDS_RENDERER_POSTPROCESSOR_SURFACE_PROJECTION_VISUALIZER_H

#include <memory>

#include "renderer/dag/dag_operation.h"
#include "renderer/projection_space/project_surface.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief DoVisualizeSurfaceProjection A post processing graphics pipeline for visualizing a
 * rendered surface parameter map.
 *
 * @param parameter_to_visualize Selects the surface parameter to be visualized.
 * @param surface_projection The surface parameter maps to select parameter from.
 * @param transfer_context Transfer context.
 * @param target The target stage which stores a map of lighting parameters (light inputs).
 */
void DoVisualizeSurfaceProjection(LightInputsRendererParameters::InputType parameter_to_visualize,
                                  DagOperation *surface_projection,
                                  TransferContext *transfer_context, DagOperation *target);

}  // namespace e8

#endif  // ISLANDS_RENDERER_POSTPROCESSOR_SURFACE_PROJECTION_VISUALIZER_H
