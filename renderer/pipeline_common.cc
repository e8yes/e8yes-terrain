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
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "third_party/vma/vk_mem_alloc.h"

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

VkDescriptorSet CreateDescriptorSet(VkDescriptorSetLayout layout, VulkanContext *context) {
    VkDescriptorSetAllocateInfo desc_set_alloc_info{};
    desc_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    desc_set_alloc_info.descriptorPool = context->descriptor_pool;
    desc_set_alloc_info.pSetLayouts = &layout;
    desc_set_alloc_info.descriptorSetCount = 1;

    VkDescriptorSet descriptor_set;
    assert(VK_SUCCESS ==
           vkAllocateDescriptorSets(context->device, &desc_set_alloc_info, &descriptor_set));

    return descriptor_set;
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
    info->rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

FrameBufferAttachment::FrameBufferAttachment(bool swap_chain_image, VulkanContext *context)
    : swap_chain_image(swap_chain_image), allocation(VK_NULL_HANDLE), desc{}, context_(context) {}

FrameBufferAttachment::~FrameBufferAttachment() {
    if (!swap_chain_image) {
        vkDestroyImageView(context_->device, view, /*pAllocator=*/nullptr);
        vmaDestroyImage(context_->allocator, image, allocation);
    }
}

std::vector<std::unique_ptr<FrameBufferAttachment>>
CreateColorAttachmentsForSwapChain(VulkanContext *context) {
    std::vector<std::unique_ptr<FrameBufferAttachment>> infos;

    for (unsigned i = 0; i < context->swap_chain_images.size(); ++i) {
        auto info = std::make_unique<FrameBufferAttachment>(/*swap_chain_image=*/true, context);

        info->image = context->swap_chain_images[i];
        info->view = context->swap_chain_image_views[i];

        info->desc.flags = 0;
        info->desc.format = context->swap_chain_image_format.format;
        info->desc.samples = VK_SAMPLE_COUNT_1_BIT;
        info->desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        info->desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        info->desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        info->desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        info->desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        info->desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        infos.emplace_back(std::move(info));
    }

    return infos;
}

std::unique_ptr<FrameBufferAttachment>
CreateColorAttachment(unsigned width, unsigned height, VkFormat format, VulkanContext *context) {
    auto info = std::make_unique<FrameBufferAttachment>(/*swap_chain_image=*/false, context);

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = format;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags =
        VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    assert(VK_SUCCESS == vmaCreateImage(context->allocator, &image_info, &allocation_create_info,
                                        &info->image, &info->allocation,
                                        /*pAllocationInfo=*/nullptr));

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.image = info->image;
    view_info.format = format;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    assert(VK_SUCCESS == vkCreateImageView(context->device, &view_info,
                                           /*pAllocator=*/nullptr, &info->view));

    info->desc.flags = 0;
    info->desc.format = format;
    info->desc.samples = VK_SAMPLE_COUNT_1_BIT;
    info->desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    info->desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    info->desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    info->desc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    return info;
}

std::unique_ptr<FrameBufferAttachment>
CreateDepthAttachment(unsigned width, unsigned height, bool samplable, VulkanContext *context) {
    auto info = std::make_unique<FrameBufferAttachment>(/*swap_chain_image=*/false, context);

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = VK_FORMAT_D32_SFLOAT;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (samplable) {
        image_info.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }

    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocation_create_info.requiredFlags =
        VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    assert(VK_SUCCESS == vmaCreateImage(context->allocator, &image_info, &allocation_create_info,
                                        &info->image, &info->allocation,
                                        /*pAllocationInfo=*/nullptr));

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.image = info->image;
    view_info.format = VK_FORMAT_D32_SFLOAT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    assert(VK_SUCCESS == vkCreateImageView(context->device, &view_info,
                                           /*pAllocator=*/nullptr, &info->view));

    info->desc.flags = 0;
    info->desc.format = VK_FORMAT_D32_SFLOAT;
    info->desc.samples = VK_SAMPLE_COUNT_1_BIT;
    info->desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    info->desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    info->desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    info->desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (samplable) {
        info->desc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    } else {
        info->desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    return info;
}

RenderPass::RenderPass(VulkanContext *context) : pass(VK_NULL_HANDLE), context_(context) {}

RenderPass::~RenderPass() { vkDestroyRenderPass(context_->device, pass, /*pAllocator=*/nullptr); }

std::unique_ptr<RenderPass>
CreateRenderPass(std::vector<VkAttachmentDescription> const &color_attachment_descs,
                 std::optional<VkAttachmentDescription> const &depth_attachment_desc,
                 VulkanContext *context) {
    auto info = std::make_unique<RenderPass>(context);

    // Creates render pass & subpass.
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    // Collects and sets attachment references.
    std::vector<VkAttachmentReference> color_attachment_refs;
    for (unsigned i = 0; i < color_attachment_descs.size(); ++i) {
        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        color_attachment_ref.attachment = i;

        color_attachment_refs.push_back(color_attachment_ref);
    }
    if (!color_attachment_refs.empty()) {
        subpass.pColorAttachments = color_attachment_refs.data();
        subpass.colorAttachmentCount = color_attachment_refs.size();
    }

    VkAttachmentReference depth_attachment_ref{};
    if (depth_attachment_desc.has_value()) {
        depth_attachment_ref.attachment = color_attachment_descs.size();
        depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        subpass.pDepthStencilAttachment = &depth_attachment_ref;
    }

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.subpassCount = 1;

    // Collects and sets attachment descriptions.
    std::vector<VkAttachmentDescription> attachment_descs(color_attachment_descs.begin(),
                                                          color_attachment_descs.end());
    if (depth_attachment_desc.has_value()) {
        attachment_descs.push_back(*depth_attachment_desc);
    }
    if (!attachment_descs.empty()) {
        render_pass_info.pAttachments = attachment_descs.data();
        render_pass_info.attachmentCount = attachment_descs.size();
    }

    assert(VK_SUCCESS == vkCreateRenderPass(context->device, &render_pass_info,
                                            /*pAllocator=*/nullptr, &info->pass));

    return info;
}

GraphicsPipeline::GraphicsPipeline(VulkanContext *context)
    : pipeline(VK_NULL_HANDLE), context_(context) {}

GraphicsPipeline::~GraphicsPipeline() {
    vkDestroyPipeline(context_->device, pipeline, /*pAllocator=*/nullptr);
}

std::unique_ptr<GraphicsPipeline>
CreateGraphicsPipeline(RenderPass const &render_pass, ShaderStages const &shader_stages,
                       ShaderUniformLayout const &uniform_layout, VertexInputInfo const &inputs,
                       FixedStageConfig const &fixed_stage_config, VulkanContext *context) {
    auto info = std::make_unique<GraphicsPipeline>(context);

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    if (!shader_stages.stages.empty()) {
        pipeline_info.pStages = shader_stages.stages.data();
        pipeline_info.stageCount = shader_stages.stages.size();
    }
    pipeline_info.pVertexInputState = &inputs.state;
    pipeline_info.pInputAssemblyState = &fixed_stage_config.input_assembly;
    pipeline_info.pViewportState = &fixed_stage_config.viewport_state;
    pipeline_info.pRasterizationState = &fixed_stage_config.rasterizer;
    pipeline_info.pMultisampleState = &fixed_stage_config.multisampling;
    pipeline_info.pDepthStencilState = &fixed_stage_config.depth_stencil;
    pipeline_info.pColorBlendState = &fixed_stage_config.color_blending;
    pipeline_info.layout = uniform_layout.layout;
    pipeline_info.renderPass = render_pass.pass;

    assert(VK_SUCCESS == vkCreateGraphicsPipelines(context->device,
                                                   /*pipelineCache=*/VK_NULL_HANDLE,
                                                   /*createInfoCount=*/1, &pipeline_info,
                                                   /*pAllocator=*/nullptr, &info->pipeline));

    return info;
}

FrameBuffer::FrameBuffer(unsigned width, unsigned height, VulkanContext *context)
    : width(width), height(height), context_(context) {}

FrameBuffer::~FrameBuffer() {
    vkDestroyFramebuffer(context_->device, buffer, /*pAllocator=*/nullptr);
}

std::unique_ptr<FrameBuffer> CreateFrameBuffer(RenderPass const &render_pass, unsigned width,
                                               unsigned height,
                                               std::vector<VkImageView> const &color_attachments,
                                               std::optional<VkImageView> const &depth_attachment,
                                               VulkanContext *context) {
    auto info = std::make_unique<FrameBuffer>(width, height, context);

    // Creates frame buffer.
    VkFramebufferCreateInfo frame_buffer_info{};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.renderPass = render_pass.pass;
    frame_buffer_info.width = width;
    frame_buffer_info.height = height;
    frame_buffer_info.layers = 1;

    // Collects and sets attachment views.
    std::vector<VkImageView> attachment_views(color_attachments.begin(), color_attachments.end());
    if (depth_attachment.has_value()) {
        attachment_views.push_back(*depth_attachment);
    }

    if (!attachment_views.empty()) {
        frame_buffer_info.pAttachments = attachment_views.data();
        frame_buffer_info.attachmentCount = attachment_views.size();
    }

    assert(VK_SUCCESS == vkCreateFramebuffer(context->device, &frame_buffer_info,
                                             /*pAllocator=*/nullptr, &info->buffer));

    // Set clear values defaults.
    if (!color_attachments.empty()) {
        VkClearValue color_value{};
        info->clear_values.push_back(color_value);
    }
    if (depth_attachment.has_value()) {
        VkClearValue depth_value{};
        depth_value.depthStencil.depth = 0.0f;
        depth_value.depthStencil.stencil = 0.0f;
        info->clear_values.push_back(depth_value);
    }

    return info;
}

UniformBuffer::UniformBuffer(unsigned size, VulkanContext *context) : size(size), context(context) {
    VkBufferCreateInfo uniform_buffer_info{};
    uniform_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    uniform_buffer_info.size = size;
    uniform_buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    assert(VK_SUCCESS == vmaCreateBuffer(context->allocator, &uniform_buffer_info, &allocation_info,
                                         &buffer, &allocation, /*pAllocatorInfo=*/nullptr));
}

UniformBuffer::~UniformBuffer() { vmaDestroyBuffer(context->allocator, buffer, allocation); }

std::unique_ptr<UniformBuffer> CreateUniformBuffer(unsigned size, VulkanContext *context) {
    return std::make_unique<UniformBuffer>(size, context);
}

ImageSampler::ImageSampler(VulkanContext *context) : context(context) {}

ImageSampler::~ImageSampler() {
    vkDestroySampler(context->device, sampler, /*pAllocator=*/nullptr);
}

std::unique_ptr<ImageSampler> CreateReadBackSampler(VulkanContext *context) {
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.magFilter = VK_FILTER_NEAREST;
    sampler_info.minFilter = VK_FILTER_NEAREST;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    auto info = std::make_unique<ImageSampler>(context);
    assert(VK_SUCCESS ==
           vkCreateSampler(context->device, &sampler_info, /*pAllocator=*/nullptr, &info->sampler));

    return info;
}

std::unique_ptr<ImageSampler> CreateTextureSampler(VulkanContext *context) {
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    auto info = std::make_unique<ImageSampler>(context);
    assert(VK_SUCCESS ==
           vkCreateSampler(context->device, &sampler_info, /*pAllocator=*/nullptr, &info->sampler));

    return info;
}

DescriptorSets::DescriptorSets(VulkanContext *context) : context(context) {}

DescriptorSets::~DescriptorSets() {
    vkFreeDescriptorSets(context->device, context->descriptor_pool,
                         /*descriptorSetCount=*/1, &frame);
    vkFreeDescriptorSets(context->device, context->descriptor_pool,
                         /*descriptorSetCount=*/1, &pass);
    vkFreeDescriptorSets(context->device, context->descriptor_pool,
                         /*descriptorSetCount=*/1, &drawable);
}

std::unique_ptr<DescriptorSets> CreateDescriptorSets(ShaderUniformLayout const &uniform_layout,
                                                     VulkanContext *context) {
    auto info = std::make_unique<DescriptorSets>(context);

    info->drawable = CreateDescriptorSet(uniform_layout.per_drawable_desc_set, context);
    info->pass = CreateDescriptorSet(uniform_layout.per_pass_desc_set, context);
    info->frame = CreateDescriptorSet(uniform_layout.per_frame_desc_set, context);

    return info;
}

void WriteUniformBufferDescriptor(void *data, UniformBuffer const &uniform_buffer,
                                  VkDescriptorSet descriptor_set, unsigned binding,
                                  VulkanContext *context) {
    //
    void *staging_buffer;
    vmaMapMemory(context->allocator, uniform_buffer.allocation, &staging_buffer);
    memcpy(staging_buffer, data, uniform_buffer.size);
    vmaUnmapMemory(context->allocator, uniform_buffer.allocation);

    //
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = uniform_buffer.buffer;
    buffer_info.offset = 0;
    buffer_info.range = uniform_buffer.size;

    VkWriteDescriptorSet write_descriptor{};
    write_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write_descriptor.dstSet = descriptor_set;
    write_descriptor.dstBinding = binding;
    write_descriptor.descriptorCount = 1;
    write_descriptor.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(context->device, /*descriptorWriteCount=*/1, &write_descriptor,
                           /*descriptorCopyCount=*/0, /*pDescriptorCopies=*/nullptr);
}

void WriteImageDescriptor(VkImageView image_view, ImageSampler const &image_sampler,
                          VkDescriptorSet descriptor_set, unsigned binding,
                          VulkanContext *context) {
    VkDescriptorImageInfo image_info{};
    image_info.sampler = image_sampler.sampler;
    image_info.imageView = image_view;
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet write_descriptor{};
    write_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write_descriptor.dstSet = descriptor_set;
    write_descriptor.dstBinding = binding;
    write_descriptor.descriptorCount = 1;
    write_descriptor.pImageInfo = &image_info;

    vkUpdateDescriptorSets(context->device, /*descriptorWriteCount=*/1, &write_descriptor,
                           /*descriptorCopyCount=*/0, /*pDescriptorCopies=*/nullptr);
}

} // namespace e8
