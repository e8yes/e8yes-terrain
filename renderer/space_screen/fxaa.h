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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_FXAA_H
#define ISLANDS_RENDERER_SPACE_SCREEN_FXAA_H

#include <memory>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"

namespace e8 {

/**
 * @brief DoFxaa Reduces edge aliasing artifact through a screen space processing technique devised
 * by Timothy Lottes.
 *
 * @param ldr_image An LDR RGB image where the alpha channel is populated with the color's
 * luminance value.
 * @param color_image_output The output which stores the anti-aliased image.
 * @param session The DAG context.
 * @return The operation which performs anti-aliasing to the LDR color image.
 */
DagOperationInstance
DoFxaa(DagOperationInstance ldr_image,
       const std::shared_ptr<GraphicsPipelineOutputInterface> &color_image_output,
       DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_FXAA_H
