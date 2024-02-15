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

#ifndef ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_DEPTH_H
#define ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_DEPTH_H

#include <memory>
#include <vector>

#include "renderer/basic/projection.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/drawable/collection.h"

namespace e8 {

/**
 * @brief DoProjectNdcDepth Schedules a graphics pipeline for rendering an NDC depth map (A mapping
 * of the cloest NDC depth value at each projected pixel). The size of the depth map is the same as
 * that of the frame.
 *
 * @param drawable_collection A collection of drawables to project to screen space to find the
 * nearest depth.
 * @param projection Defines how drawables should be projected to the depth map.
 * @param width
 * @param height
 * @param dag DAG context.
 * @return The operation which stores the rendered depth map. The output of the operation contains a
 * depth attachment only.
 */
DagOperationInstance DoProjectNdcDepth(DrawableCollection *drawable_collection,
                                       PerspectiveProjection const &projection, unsigned width,
                                       unsigned height, DagContext *dag);

/**
 * @brief DoProjectLinearDepth Schedules a graphics pipeline for rendering a linear depth map (A
 * mapping of the cloest linear depth value at each projected pixel).
 *
 * @param drawable_collection A collection of drawables to project to screen space to find the
 * nearest depth.
 * @param projection Defines how drawables should be projected to the depth map.
 * @param width The width of the depth map.
 * @param height The height of the depth map.
 * @param dependent_op Some dependent operation.
 * @param dag DAG context.
 * @return The operation which stores the rendered linear depth map. The output of the operation
 * contains both a depth attachment and a color attachment. The depth attachment stores the NDC
 * depth whereas the color attachment stores the linear depth.
 */
DagOperationInstance DoProjectLinearDepth(DrawableCollection *drawable_collection,
                                          PerspectiveProjection const &projection, unsigned width,
                                          unsigned height, DagOperationInstance const dependent_op,
                                          DagContext *dag);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_DEPTH_H
