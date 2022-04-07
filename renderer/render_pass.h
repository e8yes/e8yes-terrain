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

#ifndef ISLANDS_RENDERER_RENDER_PASS_H
#define ISLANDS_RENDERER_RENDER_PASS_H

#include <array>
#include <functional>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/descriptor_set.h"
#include "renderer/descriptor_set_texture.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_output.h"
#include "renderer/texture_group.h"
#include "renderer/vram_geometry.h"
#include "renderer/vram_texture.h"
#include "resource/buffer_texture.h"

namespace e8 {

/**
 * @brief StartRenderPass Allocates a command buffer and makes preparation for the render pass.
 *
 * @param pass The render pass to run.
 * @param frame_buffer The frame buffer to store the output of this render pass.
 * @param context Contextual Vulkan handles.
 * @return The allocated command buffer.
 */
VkCommandBuffer StartRenderPass(RenderPass const &pass, FrameBuffer const &frame_buffer,
                                VulkanContext *context);

/**
 * @brief FinishRenderPass Submits the command buffer to the graphics queue and returns a barrier
 * for this render pass.
 *
 * @param cmds The command buffer to be submitted.
 * @param prerequisites Barrier of previous tasks that need to be complete before this render pass
 * can be run.
 * @param fence Optional. If provided, the fence will be signaled when this render pass is finished.
 * @param context Contextual Vulkan handles.
 * @return The task barrier for this render pass.
 */
std::unique_ptr<GpuBarrier> FinishRenderPass(VkCommandBuffer cmds, GpuBarrier const &prerequisites,
                                             VkFence fence, VulkanContext *context);

/**
 * @brief The RenderPassConfiguratorInterface class For configuring what resources go in a render
 * pass and what shader uniform setup to apply to each drawable.
 */
class RenderPassConfiguratorInterface {
  public:
    RenderPassConfiguratorInterface();
    virtual ~RenderPassConfiguratorInterface();

    /**
     * @brief IncludeDrawable Whether to include the specified drawable to the render pass?
     */
    virtual bool IncludeDrawable(DrawableInstance const &drawable) const;

    /**
     * @brief TexturesOf Textures of the specified drawable needed by the render pass.
     */
    virtual TextureSelector TexturesOf(DrawableInstance const &drawable) const;

    /**
     * @brief SetUniformsFor Sets the uniform variables for drawing the specified drawable.
     */
    virtual void PushConstant(DrawableInstance const &drawable, VkCommandBuffer cmds) const;
};

/**
 * @brief RenderDrawables Renders an array of drawables with the specified graphics pipeline. Note,
 * it doesn't actually draw the drawables but only adds Vulkan commands to the command buffer.
 *
 * @param drawables The array of drawables to be rendered.
 * @param pipeline The graphics pipeline to use for the rendering.
 * @param configurator Specifies how to set up the rendering.
 * @param tex_desc_set_cache The texture descriptor set cache.
 * @param geo_vram The geometry VRAM transferer.
 * @param tex_vram The texture VRAM transferer.
 * @param cmds The command buffer to which draw commands will be added.
 */
void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                     RenderPassConfiguratorInterface const &configurator,
                     TextureDescriptorSetCache *tex_desc_set_cache, GeometryVramTransfer *geo_vram,
                     TextureVramTransfer *tex_vram, VkCommandBuffer cmds);

// Represents a function which sets the value of uniform variables for post processing.
using SetPostProcessorUniformsFn =
    std::function<void(ShaderUniformLayout const &uniform_layout, VkCommandBuffer cmds)>;

/**
 * @brief PostProcess Renders a quad that fills the pipeline output.
 *
 * @param pipeline The graphics pipeline to use for the post processing.
 * @param set_uniforms_fn A custom function to set uniform variables for the post processing
 * fragment shader.
 * @param cmds The command buffer to which draw commands will be added.
 */
void PostProcess(GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                 SetPostProcessorUniformsFn const &set_uniforms_fn, VkCommandBuffer cmds);

} // namespace e8

#endif // ISLANDS_RENDERER_RENDER_PASS_H
