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

#ifndef ISLANDS_RENDERER_PIPELINE_H
#define ISLANDS_RENDERER_PIPELINE_H

#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"

namespace e8 {

/**
 * @brief The GraphicsPipeline struct Stores a Vulkan object describing the graphics pipeline. It
 * will clean up the pipeline resource by the end of its lifecycle.
 */
struct GraphicsPipeline {
    /**
     * @brief GraphicsPipeline Should be created only by calling CreatePipeline().
     */
    explicit GraphicsPipeline(VulkanContext *context);
    ~GraphicsPipeline();

    GraphicsPipeline(GraphicsPipeline const &) = delete;
    GraphicsPipeline(GraphicsPipeline &&) = delete;

    // A full Vulkan object storing information of a graphics pipeline.
    VkPipeline pipeline;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreatePipeline Creates a graphics pipeline. This function will always return a valid
 * GraphicsPipeline structure. Any failure occurs during the graphics pipeline creation will make it
 * fail.
 *
 * @param render_pass The render pass which will be run after binding the pipeline.
 * @param shader_stages The shader programs and stage description.
 * @param inputs Description about the inputs to the vertex shader program.
 * @param layout Layout of the uniform variables declared in the shader programs.
 * @param fixed_stage_config Configurations of the fixed function stages.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the GraphicsPipeline structure.
 */
std::unique_ptr<GraphicsPipeline>
CreateGraphicsPipeline(RenderPass const &render_pass, ShaderStages const &shader_stages,
                       ShaderUniformLayout const &uniform_layout, VertexInputInfo const &inputs,
                       FixedStageConfig const &fixed_stage_config, VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_H
