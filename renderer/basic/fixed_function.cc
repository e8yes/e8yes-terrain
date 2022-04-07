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

#include <memory>
#include <vulkan/vulkan.h>

#include "renderer/basic/fixed_function.h"

namespace e8 {

FixedStageConfig::FixedStageConfig()
    : input_assembly{}, rasterizer{}, multisampling{}, depth_stencil{}, color_blending{},
      viewport_state{}, color_blending_attachment{}, viewport{}, scissor{} {}

FixedStageConfig::~FixedStageConfig() {}

std::unique_ptr<FixedStageConfig> CreateFixedStageConfig(VkPolygonMode polygon_mode,
                                                         VkCullModeFlags cull_mode,
                                                         bool enable_depth_test,
                                                         unsigned render_target_width,
                                                         unsigned render_target_height) {
    auto info = std::make_unique<FixedStageConfig>();

    info->input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info->input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info->input_assembly.primitiveRestartEnable = VK_FALSE;

    info->rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info->rasterizer.depthClampEnable = VK_FALSE;
    info->rasterizer.rasterizerDiscardEnable = VK_FALSE;
    info->rasterizer.polygonMode = polygon_mode;
    info->rasterizer.lineWidth = 1.0f;
    info->rasterizer.cullMode = cull_mode;
    info->rasterizer.frontFace =
        VK_FRONT_FACE_CLOCKWISE; // Counter-clockwise in viewspace, but clockwise after project.
    info->rasterizer.depthBiasEnable = VK_FALSE;
    info->rasterizer.depthBiasConstantFactor = 0.0f;
    info->rasterizer.depthBiasClamp = 0.0f;
    info->rasterizer.depthBiasSlopeFactor = 0.0f;

    info->multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info->multisampling.sampleShadingEnable = VK_FALSE;
    info->multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    info->multisampling.minSampleShading = 1.0f;
    info->multisampling.alphaToCoverageEnable = VK_FALSE;
    info->multisampling.alphaToOneEnable = VK_FALSE;

    info->depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    info->depth_stencil.depthTestEnable = enable_depth_test ? VK_TRUE : VK_FALSE;
    info->depth_stencil.depthWriteEnable = enable_depth_test ? VK_TRUE : VK_FALSE;
    info->depth_stencil.depthCompareOp =
        enable_depth_test ? VK_COMPARE_OP_GREATER_OR_EQUAL : VK_COMPARE_OP_ALWAYS;
    info->depth_stencil.depthBoundsTestEnable = VK_FALSE;
    info->depth_stencil.minDepthBounds = 0.0f;
    info->depth_stencil.maxDepthBounds = 1.0f;
    info->depth_stencil.stencilTestEnable = VK_FALSE;

    info->color_blending_attachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    info->color_blending_attachment.blendEnable = VK_FALSE;

    info->color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info->color_blending.logicOpEnable = VK_FALSE;
    info->color_blending.logicOp = VK_LOGIC_OP_COPY;
    info->color_blending.attachmentCount = 1;
    info->color_blending.pAttachments = &info->color_blending_attachment;

    info->viewport.width = render_target_width;
    info->viewport.height = render_target_height;
    info->viewport.minDepth = 0.0f;
    info->viewport.maxDepth = 1.0f;

    info->scissor.extent.width = render_target_width;
    info->scissor.extent.height = render_target_height;

    info->viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info->viewport_state.viewportCount = 1;
    info->viewport_state.pViewports = &info->viewport;
    info->viewport_state.scissorCount = 1;
    info->viewport_state.pScissors = &info->scissor;

    return info;
}

} // namespace e8
