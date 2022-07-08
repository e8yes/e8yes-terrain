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

#ifndef ISLANDS_RENDERER_POSTPROCESSOR_SURFAEC_PROJECTION_VISUALIZER_H
#define ISLANDS_RENDERER_POSTPROCESSOR_SURFAEC_PROJECTION_VISUALIZER_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/pipeline/project_surface.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief DoVisualizeSurfaceProjection A post processing graphics pipeline for visualizing a
 * rendered surface parameter map.
 *
 * @param parameter_to_visualize Selects the parameter to be visualized.
 * @param surface_projection The surface parameter maps to select parameter from.
 * @param desc_set_allocator Descriptor set allocator.
 * @param context Contextual Vulkan handles.
 * @param target The target stage which stores a map of lighting parameters (light inputs).
 */
void DoVisualizeSurfaceProjection(LightInputsRendererParameters::InputType parameter_to_visualize,
                                  PipelineStage *surface_projection,
                                  DescriptorSetAllocator *desc_set_allocator,
                                  VulkanContext *context, PipelineStage *target);

}  // namespace e8

#endif  // ISLANDS_RENDERER_POSTPROCESSOR_SURFAEC_PROJECTION_VISUALIZER_H
