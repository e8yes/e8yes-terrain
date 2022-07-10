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

#ifndef ISLANDS_RENDERER_PIPELINE_DEPTH_MAP_H
#define ISLANDS_RENDERER_PIPELINE_DEPTH_MAP_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "content/structure.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/query/collection.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief CreateDepthMapStage Creates a depth map pipeline stage with a 32-bit depth-only output in
 * the specified dimension.
 *
 * @param width The width of the depth map output.
 * @param height The height of the depth map output.
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the depth map output.
 */
std::unique_ptr<PipelineStage> CreateDepthMapStage(unsigned width, unsigned height,
                                                   VulkanContext *context);

/**
 * @brief DoDepthMapping Schedules a graphics pipeline for rendering a depth map.
 *
 * @param drawables A collection of drawables to project to screen space to find the nearest depth.
 * @param projection Defines how drawables should be projected to the depth map.
 * @param tex_desc_set_cache Texture descriptor cache.
 * @param geo_vram The geometry VRAM transferer.
 * @param tex_vram The texture VRAM transferer.
 * @param context Contextual Vulkan handles.
 * @param first_stage The frame's first stage.
 * @param target The target stage which stores the rendered depth map. It should be created using
 * CreateDepthMapStage().
 */
void DoDepthMapping(DrawableCollection *drawables, PerspectiveProjection const &projection,
                    TextureDescriptorSetCache *tex_desc_set_cache, GeometryVramTransfer *geo_vram,
                    TextureVramTransfer *tex_vram, VulkanContext *context,
                    PipelineStage *first_stage, PipelineStage *target);

}  // namespace e8

#endif  // ISLANDS_RENDERER_PIPELINE_DEPTH_MAP_H
