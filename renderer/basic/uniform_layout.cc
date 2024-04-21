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
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/uniform_layout.h"

namespace e8 {

ShaderUniformLayout::ShaderUniformLayout(VulkanContext *context)
    : layout(VK_NULL_HANDLE), context_(context) {}

ShaderUniformLayout::~ShaderUniformLayout() {
    for (auto &descriptor_set_layout : descriptor_set_layouts) {
        vkDestroyDescriptorSetLayout(context_->device, descriptor_set_layout,
                                     /*pAllocator=*/nullptr);
    }

    vkDestroyPipelineLayout(context_->device, layout, /*pAllocator=*/nullptr);
}

std::unique_ptr<ShaderUniformLayout> CreateShaderUniformLayout(
    std::optional<VkPushConstantRange> const &push_constant,
    std::vector<ShaderUniformPackageBindings> const &uniform_packages_bindings,
    VulkanContext *context) {
    auto info = std::make_unique<ShaderUniformLayout>(context);

    // Creates descriptor set layouts for the uniform packages.
    info->descriptor_set_layouts.resize(uniform_packages_bindings.size());
    for (unsigned i = 0; i < uniform_packages_bindings.size(); ++i) {
        VkDescriptorSetLayoutCreateInfo desc_set_info{};
        desc_set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        desc_set_info.pBindings = uniform_packages_bindings[i].data();
        desc_set_info.bindingCount = uniform_packages_bindings[i].size();

        assert(VK_SUCCESS == vkCreateDescriptorSetLayout(context->device, &desc_set_info,
                                                         /*pAllocator=*/nullptr,
                                                         &info->descriptor_set_layouts[i]));
    }

    VkPipelineLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    // Sets push constant layout.
    info->push_constant_range = push_constant;
    if (info->push_constant_range.has_value()) {
        layout_info.pPushConstantRanges = &info->push_constant_range.value();
        layout_info.pushConstantRangeCount = 1;
    }

    layout_info.pSetLayouts = info->descriptor_set_layouts.data();
    layout_info.setLayoutCount = info->descriptor_set_layouts.size();

    assert(VK_SUCCESS == vkCreatePipelineLayout(context->device, &layout_info,
                                                /*pAllocator=*/nullptr, &info->layout));

    return info;
}

} // namespace e8
