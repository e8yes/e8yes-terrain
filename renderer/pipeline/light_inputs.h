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

#ifndef ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_H
#define ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_H

#include <memory>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"

namespace e8 {

/**
 * @brief The LightInputsColorOutput enum Index to the light input parameter images.
 */
enum LightInputsColorOutput {
    // The image which stores the normal vector and the roughness factor.
    LICO_NORMAL_ROUGHNESS,

    // The image which stores the albdeo color (in RGB) and the metallic factor.
    LICO_ALBEDO_METALLIC,

    // The number of parameter images.
    LightInputsColorOutputCount,
};

/**
 * @brief CreateLightInputsOutput Creates a light inputs pipeline stage with two 32-bit parameter
 * maps and a 32-bit depth map output in the specified dimension.
 *
 * @param width The width of the light parameter map output.
 * @param height The width of the light parameter map output.
 * @param context Contextual Vulkan handles.
 * @return The light input stage.
 */
std::unique_ptr<PipelineStage> CreateLightInputsStage(unsigned width, unsigned height,
                                                      VulkanContext *context);

/**
 * @brief DoGenerateLightInputs Schedules a graphics pipeline to generate a map of geometry
 * information. These information are essential for lighting computation. They are: normal vector,
 * roughness factor and depth.
 *
 * @param drawables An array of drawables to be rendered onto the light inputs map.
 * @param projection Defines how drawables should be projected to the light inputs map.
 * @param tex_desc_set_cache Texture descriptor cache.
 * @param geo_vram The geometry VRAM transferer.
 * @param tex_vram The texture VRAM transferer.
 * @param context Contextual Vulkan handles.
 * @param first_stage The frame's first stage.
 * @param target The target stage which stores the rendered light inputs. It should be created using
 * CreateLightInputsStage().
 */
void DoGenerateLightInputs(std::vector<DrawableInstance> const &drawables,
                           ProjectionInterface const &projection,
                           TextureDescriptorSetCache *tex_desc_set_cache,
                           GeometryVramTransfer *geo_vram, TextureVramTransfer *tex_vram,
                           VulkanContext *context, PipelineStage *first_stage,
                           PipelineStage *target);

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_H
