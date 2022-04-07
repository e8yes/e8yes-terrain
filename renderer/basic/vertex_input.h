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

#ifndef ISLANDS_RENDERER_VERTEX_INPUT_H
#define ISLANDS_RENDERER_VERTEX_INPUT_H

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace e8 {

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
 * @return A valid unique pointer to the VertexInputInfo structure.
 */
std::unique_ptr<VertexInputInfo>
CreateVertexInputState(std::vector<VkVertexInputAttributeDescription> const &input_attributes);

} // namespace e8

#endif // ISLANDS_RENDERER_VERTEX_INPUT_H
