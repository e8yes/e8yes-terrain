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

#ifndef ISLANDS_RENDERER_POSTPROCESSOR_DEPTH_PROJECTION_VISUALIZER_H
#define ISLANDS_RENDERER_POSTPROCESSOR_DEPTH_PROJECTION_VISUALIZER_H

#include <memory>
#include <optional>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief DoVisualizeDepthProjection Schedules a post processing graphics pipeline for visualizing a
 * rendered depth map.
 *
 * @param alpha When alpha is zero, the depth map is visualized using the raw NDC value. When it is
 * set to one, the value is corrected to display the true depth.
 * @param projection The projection used to create the depth map. If alpha is non-zero, this
 * argument is required in order to get the correct result.
 * @param depth_map_stage The depth map to be visualized.
 * @param desc_set_allocator Descriptor set allocator.
 * @param context Contextual Vulkan handles.
 * @param target The target stage which stores the visualized depth map grayscale image.
 */
void DoVisualizeDepthProjection(float alpha, std::optional<PerspectiveProjection> projection,
                                PipelineStage *depth_map_stage,
                                DescriptorSetAllocator *desc_set_allocator, VulkanContext *context,
                                PipelineStage *target);

}  // namespace e8

#endif  // ISLANDS_RENDERER_POSTPROCESSOR_DEPTH_PROJECTION_VISUALIZER_H
