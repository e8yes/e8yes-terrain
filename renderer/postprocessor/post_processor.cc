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
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pass/rasterize.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

struct ViewportDimension {
    float viewport_width;
    float viewport_height;
};

VkDescriptorSetLayoutBinding PostProcessorViewportBinding() {
    VkDescriptorSetLayoutBinding viewport_binding;
    viewport_binding.binding = 0;
    viewport_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    viewport_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    viewport_binding.descriptorCount = 1;

    return viewport_binding;
}

std::unique_ptr<ShaderUniformLayout>
UniformLayout(unsigned input_image_count, unsigned push_constant_size, VulkanContext *context) {
    std::vector<VkDescriptorSetLayoutBinding> input_images_layouts(input_image_count);

    for (unsigned i = 0; i < input_image_count; ++i) {
        VkDescriptorSetLayoutBinding input_image_binding{};
        input_image_binding.binding = i;
        input_image_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        input_image_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        input_image_binding.descriptorCount = 1;

        input_images_layouts[i] = input_image_binding;
    }

    std::optional<VkPushConstantRange> push_constants;
    if (push_constant_size > 0) {
        VkPushConstantRange range{};
        range.offset = 0;
        range.size = push_constant_size;
        range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        push_constants = range;
    }

    return CreateShaderUniformLayout(
        push_constants,
        /*per_frame_desc_set=*/
        std::vector<VkDescriptorSetLayoutBinding>{PostProcessorViewportBinding()},
        /*per_pass_desc_set=*/input_images_layouts,
        /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
}

void ConfigureViewportDimensionUniformVariable(ShaderUniformLayout const &uniform_layout,
                                               DescriptorSet const &viewport_dimension_desc_set,
                                               VkCommandBuffer cmds) {
    vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                            /*firstSet=*/DescriptorSetType::DST_PER_FRAME,
                            /*descriptorSetCount=*/1, &viewport_dimension_desc_set.descriptor_set,
                            /*dynamicOffsetCount=*/0,
                            /*pDynamicOffsets=*/nullptr);
}

void ConfigureCustomUniformVariables(PostProcessorConfiguratorInterface const &configurator,
                                     ShaderUniformLayout const &uniform_layout,
                                     DescriptorSet const &input_images_desc_set,
                                     ImageSampler const &input_image_sampler,
                                     std::vector<VkImageView> *past_input_images,
                                     VulkanContext *context, VkCommandBuffer cmds) {
    if (uniform_layout.push_constant_range.has_value()) {
        std::vector<uint8_t> push_constants(uniform_layout.push_constant_range->size);
        configurator.PushConstants(&push_constants);

        vkCmdPushConstants(cmds, uniform_layout.layout,
                           /*stageFlags=*/VK_SHADER_STAGE_FRAGMENT_BIT,
                           /*offset=*/0,
                           /*size=*/push_constants.size(), push_constants.data());
    }

    std::vector<VkImageView> current_input_images(past_input_images->size());
    configurator.InputImages(&current_input_images);

    if (current_input_images != *past_input_images) {
        *past_input_images = current_input_images;

        for (unsigned i = 0; i < current_input_images.size(); ++i) {
            WriteImageDescriptor(current_input_images[i], input_image_sampler,
                                 input_images_desc_set,
                                 /*binding=*/i, context);
        }
    }

    if (!current_input_images.empty()) {
        vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                /*firstSet=*/DescriptorSetType::DST_PER_PASS,
                                /*descriptorSetCount=*/1, &input_images_desc_set.descriptor_set,
                                /*dynamicOffsetCount=*/0,
                                /*pDynamicOffsets=*/nullptr);
    }
}

} // namespace

PostProcessorConfiguratorInterface::PostProcessorConfiguratorInterface() {}

PostProcessorConfiguratorInterface::~PostProcessorConfiguratorInterface() {}

void PostProcessorConfiguratorInterface::InputImages(
    std::vector<VkImageView> * /*input_images*/) const {}

void PostProcessorConfiguratorInterface::PushConstants(
    std::vector<uint8_t> * /*push_constants*/) const {}

struct PostProcessorPipeline::PostProcessorPipelineImpl {
    PostProcessorPipelineImpl(PipelineKey key, unsigned input_image_count,
                              ShaderUniformLayout const &uniform_layout,
                              PipelineOutputInterface *output,
                              DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~PostProcessorPipelineImpl();

    PipelineKey key;

    std::unique_ptr<DescriptorSet> viewport_dimension_desc_set;
    std::unique_ptr<UniformBuffer> viewport_dimension_ubo;

    std::unique_ptr<DescriptorSet> input_images_desc_set;
    std::unique_ptr<ImageSampler> input_image_sampler;
    std::vector<VkImageView> past_input_images;
};

PostProcessorPipeline::PostProcessorPipelineImpl::PostProcessorPipelineImpl(
    PipelineKey key, unsigned input_image_count, ShaderUniformLayout const &uniform_layout,
    PipelineOutputInterface *output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : key(key), past_input_images(input_image_count) {
    // Sets up the viewport dimension uniform variable.
    viewport_dimension_desc_set = desc_set_allocator->Allocate(DescriptorType::DT_UNIFORM_BUFFER,
                                                               uniform_layout.per_frame_desc_set);

    viewport_dimension_ubo = CreateUniformBuffer(/*size=*/sizeof(ViewportDimension), context);

    ViewportDimension dimension;
    dimension.viewport_width = output->Width();
    dimension.viewport_height = output->Height();

    WriteUniformBufferDescriptor(&dimension, *viewport_dimension_ubo, *viewport_dimension_desc_set,
                                 /*binding=*/0, context);

    // Sets up the input image uniform variables.
    input_images_desc_set = desc_set_allocator->Allocate(DescriptorType::DT_COMBINED_IMAGE_SAMPLER,
                                                         uniform_layout.per_pass_desc_set);
    input_image_sampler = CreateContinuousSampler(context);

    for (auto &past_input_image : past_input_images) {
        past_input_image = VK_NULL_HANDLE;
    }
}

PostProcessorPipeline::PostProcessorPipelineImpl::~PostProcessorPipelineImpl() {}

PostProcessorPipeline::PostProcessorPipeline(
    PipelineKey const &key, std::string const &fragment_shader, unsigned input_image_count,
    unsigned push_constant_size, PipelineOutputInterface *output,
    DescriptorSetAllocator *desc_set_allocator, VulkanContext *context)
    : CachedPipelineInterface(context) {
    shader_stages_ =
        CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathPostProcessor,
                           /*fragment_shader_file_path=*/fragment_shader, context);
    uniform_layout_ = UniformLayout(input_image_count, push_constant_size, context);
    vertex_inputs_ = CreateVertexInputState(
        /*input_attributes=*/std::vector<VkVertexInputAttributeDescription>());
    fixed_stage_config_ = CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                                 /*cull_mode=*/VK_CULL_MODE_NONE,
                                                 /*enable_depth_test=*/false, output->Width(),
                                                 output->Height(), /*color_attachment_count=*/1);

    // Creates the post processing pipeline.
    pipeline_ = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                       *vertex_inputs_, *fixed_stage_config_, context);

    // Sets up additional states for defining the uniform variables.
    pimpl_ = std::make_unique<PostProcessorPipelineImpl>(key, input_image_count, *uniform_layout_,
                                                         output, desc_set_allocator, context);
}

PostProcessorPipeline::~PostProcessorPipeline() {}

PipelineKey PostProcessorPipeline::Key() const { return pimpl_->key; }

Fulfillment PostProcessorPipeline::Launch(CachedPipelineArgumentsInterface const &generic_args,
                                          std::vector<GpuPromise *> const &prerequisites,
                                          unsigned completion_signal_count,
                                          PipelineOutputInterface *output) {
    VkCommandBuffer cmds =
        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), context_);

    PostProcessorConfiguratorInterface const &configurator =
        static_cast<PostProcessorConfiguratorInterface const &>(generic_args);

    PostProcess(
        *pipeline_, *uniform_layout_,
        [this, &configurator](ShaderUniformLayout const &uniform_layout, VkCommandBuffer cmds) {
            ConfigureViewportDimensionUniformVariable(uniform_layout,
                                                      *pimpl_->viewport_dimension_desc_set, cmds);
            ConfigureCustomUniformVariables(
                configurator, uniform_layout, *pimpl_->input_images_desc_set,
                *pimpl_->input_image_sampler, &pimpl_->past_input_images, context_, cmds);
        },
        cmds);

    return FinishRenderPass(cmds, completion_signal_count, prerequisites, context_);
}

} // namespace e8
