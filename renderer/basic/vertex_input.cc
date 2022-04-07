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
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/vertex_input.h"
#include "resource/buffer_vertex.h"

namespace e8 {

VertexInputInfo::VertexInputInfo(
    std::vector<VkVertexInputAttributeDescription> const &input_attributes)
    : state{}, input_binding{}, input_attributes(input_attributes) {}

VertexInputInfo::~VertexInputInfo() {}

std::unique_ptr<VertexInputInfo>
CreateVertexInputState(std::vector<VkVertexInputAttributeDescription> const &input_attributes) {
    auto info = std::make_unique<VertexInputInfo>(input_attributes);

    info->input_binding.binding = 0;
    info->input_binding.stride = sizeof(PrimitiveVertex);
    info->input_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    info->state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (!info->input_attributes.empty()) {
        info->state.vertexBindingDescriptionCount = 1;
        info->state.pVertexBindingDescriptions = &info->input_binding;
        info->state.vertexAttributeDescriptionCount = info->input_attributes.size();
        info->state.pVertexAttributeDescriptions = info->input_attributes.data();
    }

    return info;
}

} // namespace e8
