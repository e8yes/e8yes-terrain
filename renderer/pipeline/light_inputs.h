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
#include <vector>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"

namespace e8 {

/**
 * @brief The LightInputsPipelineOutput class For storing a 32-bit RGBA color output containing the
 * geometry data as well as a 32-bit depth output.
 */
class LightInputsPipelineOutput : public PipelineOutputInterface {
  public:
    enum ColorOutput {
        NORMAL_ROUGHNESS,
        ALBEDO_METALLIC,
    };

    /**
     * @brief LightInputsPipelineOutput Constructs a light inputs map output with the specified
     * dimension.
     *
     * @param width The width of the light inputs map output.
     * @param height The height of the light inputs map output.
     * @param context Contextual Vulkan handles.
     */
    LightInputsPipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~LightInputsPipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct LightInputsPipelineOutputImpl;
    std::unique_ptr<LightInputsPipelineOutputImpl> pimpl_;
};

/**
 * @brief The LightInputsPipeline class A graphics pipeline for generating a map of geometry
 * information. These information are essential for lighting computation. They are: normal vector,
 * roughness factor and depth.
 */
class LightInputsPipeline {
  public:
    /**
     * @brief LightInputsPipeline Constructs a graphics pipeline for generate light inputs.
     *
     * @param output An object for storing the rendered geometry and depth data.
     * @param context Contextual Vulkan handles.
     */
    LightInputsPipeline(LightInputsPipelineOutput *output, VulkanContext *context);
    ~LightInputsPipeline();

    /**
     * @brief Run Runs the light inputs graphics pipeline. The pipeline can only be run when the
     * previous run was finished (indicated by the output's barrier).
     *
     * @param drawables An array of drawables to be rendered onto the light inputs map.
     * @param projection Defines how drawables should be projected to the light inputs map..
     * @param prerequisites Dependent tasks.
     * @param tex_desc_set_cache
     * @param geo_vram The geometry VRAM transferer.
     * @param tex_vram The texture VRAM transferer.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    LightInputsPipelineOutput *Run(std::vector<DrawableInstance> const &drawables,
                                   ProjectionInterface const &projection,
                                   GpuPromise const &prerequisites,
                                   TextureDescriptorSetCache *tex_desc_set_cache,
                                   GeometryVramTransfer *geo_vram, TextureVramTransfer *tex_vram);

  private:
    class LightInputsPipelineImpl;

    std::unique_ptr<LightInputsPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_H
