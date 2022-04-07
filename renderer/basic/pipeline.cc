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

#include <cassert>
#include <memory>
#include <vulkan/vulkan.h>

#include "basic/pipeline.h"
#include "common/device.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"

namespace e8 {

GraphicsPipeline::GraphicsPipeline(VulkanContext *context)
    : pipeline(VK_NULL_HANDLE), context_(context) {}

GraphicsPipeline::~GraphicsPipeline() {
    vkDestroyPipeline(context_->device, pipeline, /*pAllocator=*/nullptr);
}

std::unique_ptr<GraphicsPipeline>
CreateGraphicsPipeline(RenderPass const &render_pass, ShaderStages const &shader_stages,
                       ShaderUniformLayout const &uniform_layout, VertexInputInfo const &inputs,
                       FixedStageConfig const &fixed_stage_config, VulkanContext *context) {
    auto info = std::make_unique<GraphicsPipeline>(context);

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    if (!shader_stages.stages.empty()) {
        pipeline_info.pStages = shader_stages.stages.data();
        pipeline_info.stageCount = shader_stages.stages.size();
    }
    pipeline_info.pVertexInputState = &inputs.state;
    pipeline_info.pInputAssemblyState = &fixed_stage_config.input_assembly;
    pipeline_info.pViewportState = &fixed_stage_config.viewport_state;
    pipeline_info.pRasterizationState = &fixed_stage_config.rasterizer;
    pipeline_info.pMultisampleState = &fixed_stage_config.multisampling;
    pipeline_info.pDepthStencilState = &fixed_stage_config.depth_stencil;
    pipeline_info.pColorBlendState = &fixed_stage_config.color_blending;
    pipeline_info.layout = uniform_layout.layout;
    pipeline_info.renderPass = render_pass.pass;

    assert(VK_SUCCESS == vkCreateGraphicsPipelines(context->device,
                                                   /*pipelineCache=*/VK_NULL_HANDLE,
                                                   /*createInfoCount=*/1, &pipeline_info,
                                                   /*pAllocator=*/nullptr, &info->pipeline));

    return info;
}

} // namespace e8
