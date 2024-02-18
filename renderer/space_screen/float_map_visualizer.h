/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2023 Chifeng Wen {daviesx66@gmail.com}
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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_FLOAT_MAP_VISUALIZER_H
#define ISLANDS_RENDERER_SPACE_SCREEN_FLOAT_MAP_VISUALIZER_H

#include <memory>
#include <optional>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"

namespace e8 {

/**
 * @brief DoVisualizeFloat Schedules a screen space processing graphics pipeline to visualize a
 * float map. It scales the float map into the [0, 1] range, so it can be displayed as a grayscale
 * image.
 *
 * @param float_map The float map to be visualized. The floating point values are assumed to be
 * stored in the color attachment.
 * @param min_value Values that are less than min_value are clamped to 0.
 * @param max_value Values that are greater than min_value are clamped to 1.
 * @param session The DAG session.
 * @return The target operation which visualizes the float map as a grayscale image.
 */
DagOperationInstance DoVisualizeFloat(DagOperationInstance float_map, float min_value,
                                      float max_value, DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_FLOAT_MAP_VISUALIZER_H
