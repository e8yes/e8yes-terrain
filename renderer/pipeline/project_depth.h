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

#ifndef ISLANDS_RENDERER_PIPELINE_PROJECT_DEPTH_H
#define ISLANDS_RENDERER_PIPELINE_PROJECT_DEPTH_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "content/structure.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/query/collection.h"
#include "renderer/transfer/context.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief CreateProjectDepthStage Creates a depth projection pipeline stage with a 32-bit depth-only
 * output in the specified dimension.
 *
 * @param width The width of the depth map output.
 * @param height The height of the depth map output.
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the depth map output.
 */
std::unique_ptr<PipelineStage> CreateProjectDepthStage(unsigned width, unsigned height,
                                                       VulkanContext *context);

/**
 * @brief DoProjectDepth Schedules a graphics pipeline for rendering a depth map (A mapping of the
 * cloest NDC and linear (optional) depth value at each projected pixel).
 *
 * @param drawable_collection A collection of drawables to project to screen space to find the
 * nearest depth.
 * @param projection Defines how drawables should be projected to the depth map.
 * @param transfer_context Transfer context.
 * @param first_stage The frame's first stage.
 * @param projected_ndc_depth The target stage which stores the rendered NDC depth map. It should be
 * created using CreateDepthMapStage().
 * @param projected_linear_depth Optional. The target stage which stores the rendered linear depth
 * map. It should be created using CreateDepthMapStage().
 */
void DoProjectDepth(DrawableCollection *drawable_collection,
                    PerspectiveProjection const &projection, TransferContext *transfer_context,
                    PipelineStage *first_stage, PipelineStage *projected_ndc_depth,
                    PipelineStage *projected_linear_depth = nullptr);

}  // namespace e8

#endif  // ISLANDS_RENDERER_PIPELINE_PROJECT_DEPTH_H
