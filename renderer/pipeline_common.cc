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

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/drawable.h"
#include "renderer/pipeline_common.h"
#include "renderer/vma/vk_mem_alloc.h"

namespace e8 {
namespace {

VkShaderModule LoadShader(std::string const &file_name, VulkanContext *context) {
    // Loads SPIR-V binary BLOB from file. Note that Vulkan expects the BLOB to be 32-bit aligned.
    std::ifstream shader_file(file_name, std::ios::ate | std::ios::binary);
    assert(shader_file.is_open());

    unsigned file_size = shader_file.tellg();
    unsigned blob_int_count = (file_size + sizeof(uint32_t) - 1) / sizeof(uint32_t);
    assert(blob_int_count > 0);
    std::vector<uint32_t> blob(blob_int_count);

    shader_file.seekg(0);
    shader_file.read(reinterpret_cast<char *>(blob.data()), file_size);
    shader_file.close();

    // Uploads shader code from the BLOB and creates a new shader module.
    VkShaderModuleCreateInfo shader_info{};
    shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_info.codeSize = blob.size() * sizeof(uint32_t);
    shader_info.pCode = blob.data();

    VkShaderModule shader_module;
    assert(VK_SUCCESS == vkCreateShaderModule(context->device, &shader_info,
                                              /*pAllocator=*/nullptr, &shader_module));

    return shader_module;
}

} // namespace

ShaderStages::ShaderStages(VulkanContext *context) : context_(context) {}

ShaderStages::~ShaderStages() {
    for (auto module : modules) {
        vkDestroyShaderModule(context_->device, module, /*pAllocator=*/nullptr);
    }
}

std::unique_ptr<ShaderStages>
CreateShaderStages(std::string const &vertex_shader_file_path,
                   std::optional<std::string> const &fragment_shader_file_path,
                   VulkanContext *context) {
    auto info = std::make_unique<ShaderStages>(context);

    // Loads vertex shader.
    VkShaderModule vertex_shader = LoadShader(vertex_shader_file_path, context);

    VkPipelineShaderStageCreateInfo vertex_stage{};
    vertex_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertex_stage.module = vertex_shader;
    vertex_stage.pName = "main";

    info->modules.push_back(vertex_shader);
    info->stages.push_back(vertex_stage);

    if (fragment_shader_file_path.has_value()) {
        // Loads fragment shader when specified.
        VkShaderModule fragment_shader = LoadShader(*fragment_shader_file_path, context);

        VkPipelineShaderStageCreateInfo fragment_stage{};
        fragment_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_stage.module = fragment_shader;
        fragment_stage.pName = "main";

        info->modules.push_back(fragment_shader);
        info->stages.push_back(fragment_stage);
    }

    return info;
}

} // namespace e8
