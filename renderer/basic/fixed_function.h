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

#ifndef ISLANDS_RENDERER_FIXED_FUNCTION_H
#define ISLANDS_RENDERER_FIXED_FUNCTION_H

#include <memory>
#include <vulkan/vulkan.h>

namespace e8 {

/**
 * @brief The FixedStageConfig struct Stores the configuration of the fixed function stages
 * (clipping & rasterization).
 */
struct FixedStageConfig {
    /**
     * @brief FixedStageConfig Should be created only by calling CreateFixedStageConfig().
     */
    FixedStageConfig();
    ~FixedStageConfig();

    FixedStageConfig(FixedStageConfig const &) = delete;
    FixedStageConfig(FixedStageConfig &&) = delete;

    VkPipelineInputAssemblyStateCreateInfo input_assembly;
    VkPipelineRasterizationStateCreateInfo rasterizer;
    VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineDepthStencilStateCreateInfo depth_stencil;
    VkPipelineColorBlendStateCreateInfo color_blending;
    VkPipelineViewportStateCreateInfo viewport_state;

    VkPipelineColorBlendAttachmentState color_blending_attachment;
    VkViewport viewport;
    VkRect2D scissor;
};

/**
 * @brief CreateFixedStageConfig Creates a configuration describing the behavior of fixed function
 * stages (clipping & rasterization).
 *
 * @param polygon_mode Specifies how a polygon should be rasterized.
 * @param cull_mode Specifies if the back/front faces should be excluded.
 * @param enable_depth_test Specifies if a depth rejection should be employed.
 * @param render_target_width The width, in pixels, of the target rendering area.
 * @param render_target_height The height, in pixels, of the target rendering area.
 * @return A valid unique pointer to the FixedStageConfig structure.
 */
std::unique_ptr<FixedStageConfig> CreateFixedStageConfig(VkPolygonMode polygon_mode,
                                                         VkCullModeFlags cull_mode,
                                                         bool enable_depth_test,
                                                         unsigned render_target_width,
                                                         unsigned render_target_height);

} // namespace e8

#endif // ISLANDS_RENDERER_FIXED_FUNCTION_H
