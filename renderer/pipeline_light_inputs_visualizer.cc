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
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/descriptor_set.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_light_inputs_visualizer.h"
#include "renderer/pipeline_output.h"
#include "renderer/pipeline_post_processor.h"
#include "renderer/proto/renderer.pb.h"

namespace e8 {

struct PushConstants {
    int input_to_visualize;
};

VkPushConstantRange PushConstantRange() {
    VkPushConstantRange push_constant{};

    push_constant.offset = 0;
    push_constant.size = sizeof(PushConstants);
    push_constant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    return push_constant;
}

std::vector<VkDescriptorSetLayoutBinding> LightInputsBinding() {
    VkDescriptorSetLayoutBinding light_inputs_binding{};

    light_inputs_binding.binding = 0;
    light_inputs_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    light_inputs_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    light_inputs_binding.descriptorCount = 1;

    return std::vector<VkDescriptorSetLayoutBinding>{light_inputs_binding};
}

struct LightInputsVisualizerPipeline::LightInputsVisualizerPipelineImpl {
    LightInputsVisualizerPipelineImpl(PipelineOutputInterface *visualizer_output,
                                      DescriptorSetAllocator *desc_set_allocator,
                                      VulkanContext *context);
    ~LightInputsVisualizerPipelineImpl();

    VulkanContext *context;
    DescriptorSetAllocator *desc_set_allocator;
    std::unique_ptr<ImageSampler> light_inputs_sampler;
    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;
};

LightInputsVisualizerPipeline::LightInputsVisualizerPipelineImpl::LightInputsVisualizerPipelineImpl(
    PipelineOutputInterface *visualizer_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : context(context), desc_set_allocator(desc_set_allocator) {
    light_inputs_sampler = CreateReadBackSampler(context);

    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathLightInputsVisualizer, PushConstantRange(), LightInputsBinding(),
        visualizer_output, desc_set_allocator, context);
}

LightInputsVisualizerPipeline::LightInputsVisualizerPipelineImpl::
    ~LightInputsVisualizerPipelineImpl() {}

LightInputsVisualizerPipeline::LightInputsVisualizerPipeline(
    PipelineOutputInterface *visualizer_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : pimpl_(std::make_unique<LightInputsVisualizerPipelineImpl>(visualizer_output,
                                                                 desc_set_allocator, context)) {}

LightInputsVisualizerPipeline::~LightInputsVisualizerPipeline() {}

PipelineOutputInterface *
LightInputsVisualizerPipeline::Run(LightInputsRendererParameters::InputType input_to_visualize,
                                   LightInputsPipelineOutput const &light_inputs) {
    return pimpl_->post_processor_pipeline->Run(
        *light_inputs.barrier, /*set_uniforms_fn=*/
        [this, input_to_visualize, &light_inputs](ShaderUniformLayout const &uniform_layout,
                                                  DescriptorSet const &input_images_desc_set,
                                                  VkCommandBuffer cmds) {
            // Sets projection and visualizer parameters.
            PushConstants push_constants;
            push_constants.input_to_visualize = input_to_visualize;

            vkCmdPushConstants(cmds, uniform_layout.layout,
                               /*stageFlags=*/VK_SHADER_STAGE_FRAGMENT_BIT,
                               /*offset=*/0,
                               /*size=*/sizeof(PushConstants), &push_constants);

            // Sets the depth map input.
            WriteImageDescriptor(
                light_inputs.ColorAttachment()->view, *pimpl_->light_inputs_sampler,
                input_images_desc_set.descriptor_set, /*binding=*/0, pimpl_->context);

            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                    /*firstSet=*/DescriptorSetType::DST_PER_PASS,
                                    /*descriptorSetCount=*/1, &input_images_desc_set.descriptor_set,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        });
}

} // namespace e8
