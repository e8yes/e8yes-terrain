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

#ifndef ISLANDS_RENDERER_SPACE_PROJECTION_FILL_COLOR_H
#define ISLANDS_RENDERER_SPACE_PROJECTION_FILL_COLOR_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"

namespace e8 {

/**
 * @brief DoFillColor Creates a target image and fill it with the specified color.
 *
 * @param color The color to fill to the target.
 * @param hdr Whether to create an HDR target, or otherwise an LDR target.
 * @param image The that needs to be filled.
 * @param context Contextual Vulkan handles.
 * @param dag DAG context.
 * @return The operation which stores the image filled with the specified color.
 */
DagOperationInstance DoFillColor(vec3 const &color, bool hdr, DagOperationInstance image,
                                 DagContext *dag);

/**
 * @brief DoFillColor Like the function above, it fills an image with the specified color. However,
 * it writes to the specified color image output instead of creating a new image.
 */
DagOperationInstance
DoFillColor(vec3 const &color,
            std::shared_ptr<GraphicsPipelineOutputInterface> const &color_image_output,
            DagContext *dag);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_PROJECTION_FILL_COLOR_H
