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

#ifndef ISLANDS_RENDERER_RADIANCE_PIPELINE_H
#define ISLANDS_RENDERER_RADIANCE_PIPELINE_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief DoComputeRadiance Computes the radiance produced by the specified light source based on
 * screen-space geometric and material parameters. Note, the light source must be defined in the
 * view space where the light inputs were generated.
 *
 * @param instance The light source to compute direct radiance for.
 * @param view_projection The frustum in which the light inputs are rendered.
 * @param light_inputs Lighting parameters mapped to the screen.
 * @param shadow_maps Optional. An array of exponentiated depth maps rendered from the light
 * source's perspective. Each depth map corresponds to one of the light source's region. When it is
 * specified, this function computes the radiance the occlusion factor. Otherwise, the radiance
 * penetrates all objects.
 * @param cleared_radiance_map A zeroed-out radiance map.
 * @param desc_set_allocator Descriptor set allocator.
 * @param context Contextual Vulkan handles.
 * @param target The target stage which stores the radiance map in an HDR color image.
 */
void DoComputeRadiance(LightSourceInstance const &instance, frustum const &view_projection,
                       PipelineStage *light_inputs, std::vector<PipelineStage *> const &shadow_maps,
                       PipelineStage *cleared_radiance_map,
                       DescriptorSetAllocator *desc_set_allocator, VulkanContext *context,
                       PipelineStage *target);

} // namespace e8

#endif // ISLANDS_RENDERER_RADIANCE_PIPELINE_H
