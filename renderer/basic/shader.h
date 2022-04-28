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

#ifndef ISLANDS_RENDERER_SHADER_H
#define ISLANDS_RENDERER_SHADER_H

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

// File paths to the shader programs.
constexpr char const *kVertexShaderFilePathDepthMap = "./depth.vert.spv";
constexpr char const *kVertexShaderFilePathLightInputs = "./light_inputs.vert.spv";
constexpr char const *kVertexShaderFilePathPostProcessor = "./post_processor.vert.spv";

constexpr char const *kFragmentShaderFilePathDepthMapVisualizer = "./depth_visualizer.frag.spv";
constexpr char const *kFragmentShaderFilePathHdrAces = "./hdr_aces.frag.spv";
constexpr char const *kFragmentShaderFilePathHdrClamp = "./hdr_clamp.frag.spv";
constexpr char const *kFragmentShaderFilePathLightInputs = "./light_inputs.frag.spv";
constexpr char const *kFragmentShaderFilePathLightInputsVisualizer =
    "./light_inputs_visualizer.frag.spv";
constexpr char const *kFragmentShaderFilePathLogLuminance = "./log_luminance.frag.spv";
constexpr char const *kFragmentShaderFilePathPostProcessorEmpty = "./post_processor.frag.spv";
constexpr char const *kFragmentShaderFilePathRadiancePointLight = "./radiance_point_light.frag.spv";
constexpr char const *kFragmentShaderFilePathRadianceSpotLight = "./radiance_spot_light.frag.spv";
constexpr char const *kFragmentShaderFilePathRadianceSunLight = "./radiance_sun_light.frag.spv";

/**
 * @brief The ShaderStages struct Stores the shader handle and stage information of a graphics
 * pipeline. It will clean up shader resources by the end of its lifecycle.
 */
struct ShaderStages {
    /**
     * @brief ShaderStages Should be created only by calling CreateShaderStages().
     */
    explicit ShaderStages(VulkanContext *context);
    ~ShaderStages();

    ShaderStages(ShaderStages const &) = delete;
    ShaderStages(ShaderStages &&) = delete;

    // Shader modules loaded and the stage information attached.
    std::vector<VkShaderModule> modules;
    std::vector<VkPipelineShaderStageCreateInfo> stages;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateShaderStages Loads vertex and fragment shader programs in SPIR-V IR and prepares
 * them for a graphics pipeline. This function will always return a valid ShaderStages structure.
 * Any failure occurs in the process of loading shader programs will make it fail.
 *
 * @param vertex_shader_file_path File path to the vertex shader program. Note, vertex shader is a
 * required shader stage.
 * @param fragment_shader_file_path File path to the fragment shader program. This shader stage is
 * optional.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ShaderStages structure.
 */
std::unique_ptr<ShaderStages>
CreateShaderStages(std::string const &vertex_shader_file_path,
                   std::optional<std::string> const &fragment_shader_file_path,
                   VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_SHADER_H
