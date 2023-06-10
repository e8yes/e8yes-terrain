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

#include <vector>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/dag/dag_operation.h"

namespace e8 {

/**
 * @brief DoFillColor Fills the target with the specified color. Note, it's an asynchronous
 * function.
 *
 * @param color The color to fill to the target.
 * * @param context Contextual Vulkan handles.
 * @param first_stage The frame's first stage.
 * @param target The target to be filled.
 */
void DoFillColor(vec3 const &color, VulkanContext *context, DagOperation *first_stage,
                 DagOperation *target);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_PROJECTION_FILL_COLOR_H
