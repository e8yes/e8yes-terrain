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

#ifndef ISLANDS_RENDERER_UNIFORM_LAYOUT_H
#define ISLANDS_RENDERER_UNIFORM_LAYOUT_H

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

//
using ShaderUniformPackageBindings = std::vector<VkDescriptorSetLayoutBinding>;

/**
 * @brief The ShaderUniformLayoutInfo struct Stores a Vulkan object describing the layout of the
 * uniform variables declared in the shader programs in a graphics pipeline. It will clean up layout
 * resources by the end of its lifecycle.
 */
struct ShaderUniformLayout {
    /**
     * @brief ShaderUniformLayoutInfo Should be created only by calling CreateShaderUniformLayout().
     */
    explicit ShaderUniformLayout(VulkanContext *context);
    ~ShaderUniformLayout();

    ShaderUniformLayout(ShaderUniformLayout const &) = delete;
    ShaderUniformLayout(ShaderUniformLayout &&) = delete;

    // Push constant setup.
    std::optional<VkPushConstantRange> push_constant_range;

    // Descriptor set layouts designed for different packages of uniform data.
    std::vector<VkDescriptorSetLayout> descriptor_set_layouts;

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
 * This function will always return a valid ShaderUniformLayout structure. Any failure occurs during
 * layout creation will make it fail.
 *
 * @param push_constant Specifies the push-constant layout.
 * @param uniform_packages_bindings Descriptor set layouts designed for different packages of
 * uniform data.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ShaderUniformLayout structure.
 */
std::unique_ptr<ShaderUniformLayout> CreateShaderUniformLayout(
    std::optional<VkPushConstantRange> const &push_constant,
    std::vector<ShaderUniformPackageBindings> const &uniform_packages_bindings,
    VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_UNIFORM_LAYOUT_H
