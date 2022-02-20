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

#ifndef ISLANDS_RENDERER_PIPELINE_COMMON_H
#define ISLANDS_RENDERER_PIPELINE_COMMON_H

#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/drawable.h"
#include "renderer/vma/vk_mem_alloc.h"
#include "renderer/vram.h"

namespace e8 {

// File paths to the shader programs.
constexpr char const *kVertexShaderFilePathDepthMap = "./depth.vert.spv";
constexpr char const *kVertexShaderFilePathGeometryMap = "./geometry.vert.spv";

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

/**
 * @brief The ShaderUniformLayoutInfo struct Stores a Vulkan object describing the layout of the
 * uniform variables declared in the shader programs in a graphics pipeline.
 */
struct ShaderUniformLayout {
    /**
     * @brief ShaderUniformLayoutInfo Should be created only by calling CreateShaderUniformLayout().
     */
    explicit ShaderUniformLayout(VulkanContext *context);
    ~ShaderUniformLayout();

    ShaderUniformLayout(ShaderUniformLayout const &) = delete;
    ShaderUniformLayout(ShaderUniformLayout &&) = delete;

    // A full Vulkan object storing the layout of shader uniform variables.
    VkPipelineLayout layout;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateShaderUniformLayout Gathers information about the layout of uniform variables
 * declared in the shader programs in a graphics pipeline. Two types of uniform layouts are
 * available: push constants and descriptor sets. In particular, only one push constant uniform
 * variable is allowed, though it may be shared in both vertex and fragment shader programs.
 *
 * TODO: Allows descriptor set layout to be specified.
 *
 * @param push_constant_size The size (in bytes) of the push constant uniform variable if there is
 * one. It must be greater than zero when specified.
 * @param push_constant_accessible_stage If the argument push_constant_size is specified, the caller
 * must also provide the stages where the uniform push constant variable is accessible.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ShaderUniformLayout structure.
 */
std::unique_ptr<ShaderUniformLayout>
CreateShaderUniformLayout(std::optional<unsigned> const &push_constant_size,
                          std::optional<VkShaderStageFlags> const &push_constant_accessible_stage,
                          VulkanContext *context);

/**
 * @brief The VertexInputInfo struct Stores information about the input attributes of a vertex
 * shader program in a graphics pipeline.
 */
struct VertexInputInfo {
    /**
     * @brief VertexInputInfo Should be created only by calling VertexInputState().
     */
    explicit VertexInputInfo(
        std::vector<VkVertexInputAttributeDescription> const &input_attributes);
    ~VertexInputInfo();

    VertexInputInfo(VertexInputInfo const &) = delete;
    VertexInputInfo(VertexInputInfo &&) = delete;

    // Primary vertex attribute Vulkan description.
    VkPipelineVertexInputStateCreateInfo state;

    // These information needs to be kept alive in order for the state to hold valid memory
    // pointers.
    VkVertexInputBindingDescription input_binding;
    std::vector<VkVertexInputAttributeDescription> input_attributes;
};

/**
 * @brief VertexInputState Creates a structure storing the information about the input attributes of
 * a vertex shader program in a graphics pipeline. In particular, it describes how an attribute
 * associates with data in the vertex buffer.
 *
 * @param input_attributes An array describing all the vertex shader program's input attributes.
 * @return
 */
std::unique_ptr<VertexInputInfo>
CreateVertexInputState(std::vector<VkVertexInputAttributeDescription> const &input_attributes);

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_COMMON_H
