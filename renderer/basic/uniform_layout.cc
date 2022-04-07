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
    : per_frame_desc_set(VK_NULL_HANDLE), per_pass_desc_set(VK_NULL_HANDLE),
      per_drawable_desc_set(VK_NULL_HANDLE), layout(VK_NULL_HANDLE), context_(context) {}

ShaderUniformLayout::~ShaderUniformLayout() {
    vkDestroyDescriptorSetLayout(context_->device, per_drawable_desc_set,
                                 /*pAllocator=*/nullptr);
    vkDestroyDescriptorSetLayout(context_->device, per_pass_desc_set,
                                 /*pAllocator=*/nullptr);
    vkDestroyDescriptorSetLayout(context_->device, per_frame_desc_set,
                                 /*pAllocator=*/nullptr);

    vkDestroyPipelineLayout(context_->device, layout, /*pAllocator=*/nullptr);
}

std::unique_ptr<ShaderUniformLayout>
CreateShaderUniformLayout(std::optional<VkPushConstantRange> const &push_constant,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_frame_desc_set,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_drawable_desc_set,
                          VulkanContext *context) {
    auto info = std::make_unique<ShaderUniformLayout>(context);

    VkPipelineLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    // Sets push constant layout.
    info->push_constant_range = push_constant;
    if (push_constant.has_value()) {
        layout_info.pPushConstantRanges = &push_constant.value();
        layout_info.pushConstantRangeCount = 1;
    }

    // Creates descriptor set layouts.
    VkDescriptorSetLayoutCreateInfo per_frame_desc_set_info{};
    per_frame_desc_set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    if (!per_frame_desc_set.empty()) {
        per_frame_desc_set_info.pBindings = per_frame_desc_set.data();
        per_frame_desc_set_info.bindingCount = per_frame_desc_set.size();
    }
    assert(VK_SUCCESS == vkCreateDescriptorSetLayout(context->device, &per_frame_desc_set_info,
                                                     /*pAllocator=*/nullptr,
                                                     &info->per_frame_desc_set));

    VkDescriptorSetLayoutCreateInfo per_pass_desc_set_info{};
    per_pass_desc_set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    if (!per_pass_desc_set.empty()) {
        per_pass_desc_set_info.pBindings = per_pass_desc_set.data();
        per_pass_desc_set_info.bindingCount = per_pass_desc_set.size();
    }
    assert(VK_SUCCESS == vkCreateDescriptorSetLayout(context->device, &per_pass_desc_set_info,
                                                     /*pAllocator=*/nullptr,
                                                     &info->per_pass_desc_set));

    VkDescriptorSetLayoutCreateInfo per_drawable_desc_set_info{};
    per_drawable_desc_set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    if (!per_drawable_desc_set.empty()) {
        per_drawable_desc_set_info.pBindings = per_drawable_desc_set.data();
        per_drawable_desc_set_info.bindingCount = per_drawable_desc_set.size();
    }
    assert(VK_SUCCESS == vkCreateDescriptorSetLayout(context->device, &per_drawable_desc_set_info,
                                                     /*pAllocator=*/nullptr,
                                                     &info->per_drawable_desc_set));

    // Informs the pipeline about the descriptor set layouts.
    std::vector<VkDescriptorSetLayout> desc_set_layouts{
        info->per_frame_desc_set,
        info->per_pass_desc_set,
        info->per_drawable_desc_set,
    };

    layout_info.pSetLayouts = desc_set_layouts.data();
    layout_info.setLayoutCount = desc_set_layouts.size();

    assert(VK_SUCCESS == vkCreatePipelineLayout(context->device, &layout_info,
                                                /*pAllocator=*/nullptr, &info->layout));

    return info;
}

} // namespace e8
