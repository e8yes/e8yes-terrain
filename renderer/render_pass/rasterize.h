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

#ifndef ISLANDS_RENDER_PASS_RASTERIZE_H
#define ISLANDS_RENDER_PASS_RASTERIZE_H

#include <array>
#include <functional>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/command_buffer.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief StartRenderPass Makes preparation for the render pass.
 *
 * @param pass The render pass to run.
 * @param frame_buffer The frame buffer to store the output of this render pass.
 * @return cmds Command buffer.
 */
VkCommandBuffer StartRenderPass(RenderPass const &pass, FrameBuffer const &frame_buffer,
                                CommandBuffer *command_buffer);

/**
 * @brief FinishRenderPass Marks the completion of a render pass.
 *
 * @param cmds Command buffer.
 */
void FinishRenderPass(CommandBuffer *command_buffer);

/**
 * @brief RenderDrawables Renders an array of drawables with the specified graphics pipeline. Note,
 * it doesn't actually draw the drawables but only adds Vulkan commands to the command buffer.
 *
 * @param drawables The array of drawables to be rendered.
 * @param pipeline The graphics pipeline to use for the rendering.
 * @param configurator Specifies how to set up the rendering.
 * @param transfer_context Transfer context.
 * @param cmds The command buffer to which draw commands will be added.
 */
void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                     RenderPassConfiguratorInterface const &configurator,
                     TransferContext *transfer_context, CommandBuffer *command_buffer);

// Represents a function which sets the value of uniform variables for screen space processing.
using SetPostProcessorUniformsFn =
    std::function<void(ShaderUniformLayout const &uniform_layout, CommandBuffer *command_buffer)>;

/**
 * @brief PostProcess Renders a quad that fills the pipeline output.
 *
 * @param pipeline The graphics pipeline to use for the screen space processing.
 * @param set_uniforms_fn A custom function to set uniform variables for the screen space processing
 * fragment shader.
 * @param cmds The command buffer to which draw commands will be added.
 */
void PostProcess(GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                 SetPostProcessorUniformsFn const &set_uniforms_fn, CommandBuffer *command_buffer);

} // namespace e8

#endif // ISLANDS_RENDER_PASS_RASTERIZE_H
