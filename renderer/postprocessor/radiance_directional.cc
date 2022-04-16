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

#include "common/device.h"
#include "common/tensor.h"
#include "content/proto/light_source.pb.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/postprocessor/radiance_directional.h"
#include "renderer/transfer/descriptor_set.h"
#include "resource/common.h"

namespace e8 {
namespace {

struct PushConstants {
    vec4 dir;
    vec4 intensity;
};

VkPushConstantRange PushConstantRange() {
    VkPushConstantRange push_constant{};

    push_constant.offset = 0;
    push_constant.size = sizeof(PushConstants);
    push_constant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    return push_constant;
}

std::vector<VkDescriptorSetLayoutBinding> LightInputsBinding() {
    VkDescriptorSetLayoutBinding normal_roughness_binding{};
    normal_roughness_binding.binding = 0;
    normal_roughness_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    normal_roughness_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normal_roughness_binding.descriptorCount = 1;

    VkDescriptorSetLayoutBinding albedo_metallic_binding{};
    albedo_metallic_binding.binding = 1;
    albedo_metallic_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    albedo_metallic_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    albedo_metallic_binding.descriptorCount = 1;

    return std::vector<VkDescriptorSetLayoutBinding>{normal_roughness_binding,
                                                     albedo_metallic_binding};
}

} // namespace

struct DirectionalRadiancePipeline::DirectionalRadiancePipelineImpl {
    DirectionalRadiancePipelineImpl(UnboundedColorPipelineOutput *radiance_output,
                                    DescriptorSetAllocator *desc_set_allocator,
                                    VulkanContext *context);
    ~DirectionalRadiancePipelineImpl();

    VulkanContext *context;
    DescriptorSetAllocator *desc_set_allocator;
    std::unique_ptr<ImageSampler> light_inputs_sampler;
    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;

    LightInputsPipelineOutput const *current_light_inputs;
};

DirectionalRadiancePipeline::DirectionalRadiancePipelineImpl::DirectionalRadiancePipelineImpl(
    UnboundedColorPipelineOutput *radiance_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : context(context), desc_set_allocator(desc_set_allocator), current_light_inputs(nullptr) {
    light_inputs_sampler = CreateReadBackSampler(context);

    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathRadianceDirectional, PushConstantRange(), LightInputsBinding(),
        radiance_output, desc_set_allocator, context);
}

DirectionalRadiancePipeline::DirectionalRadiancePipelineImpl::~DirectionalRadiancePipelineImpl() {}

DirectionalRadiancePipeline::DirectionalRadiancePipeline(
    UnboundedColorPipelineOutput *radiance_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : pimpl_(std::make_unique<DirectionalRadiancePipelineImpl>(radiance_output, desc_set_allocator,
                                                               context)) {}

DirectionalRadiancePipeline::~DirectionalRadiancePipeline() {}

UnboundedColorPipelineOutput *
DirectionalRadiancePipeline::Run(SunLight const &light, ProjectionInterface const &projection,
                                 LightInputsPipelineOutput const &light_inputs) {
    PipelineOutputInterface *output = pimpl_->post_processor_pipeline->Run(
        *light_inputs.promise, /*set_uniforms_fn=*/
        [this, &light, &projection, &light_inputs](ShaderUniformLayout const &uniform_layout,
                                                   DescriptorSet const &input_images_desc_set,
                                                   VkCommandBuffer cmds) {
            vec4 world_space_direction = ToVec3(light.direction()).homo(0.0f);
            vec4 view_space_direction = projection.ViewTransform() * world_space_direction;
            vec4 input_direction = view_space_direction.normalize();

            vec4 input_intensity = ToVec3(light.intensity()).homo(0.0f);

            // Sets projection and visualizer parameters.
            PushConstants push_constants;
            push_constants.dir = input_direction;
            push_constants.intensity = input_intensity;

            vkCmdPushConstants(cmds, uniform_layout.layout,
                               /*stageFlags=*/VK_SHADER_STAGE_FRAGMENT_BIT,
                               /*offset=*/0,
                               /*size=*/sizeof(PushConstants), &push_constants);

            if (&light_inputs != pimpl_->current_light_inputs) {
                // Sets the light inputs.
                WriteImageDescriptor(
                    light_inputs.ColorAttachments()[LightInputsPipelineOutput::NORMAL_ROUGHNESS]
                        ->view,
                    *pimpl_->light_inputs_sampler, input_images_desc_set,
                    /*binding=*/0, pimpl_->context);

                WriteImageDescriptor(
                    light_inputs.ColorAttachments()[LightInputsPipelineOutput::ALBEDO_METALLIC]
                        ->view,
                    *pimpl_->light_inputs_sampler, input_images_desc_set,
                    /*binding=*/1, pimpl_->context);

                pimpl_->current_light_inputs = &light_inputs;
            }

            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                    /*firstSet=*/DescriptorSetType::DST_PER_PASS,
                                    /*descriptorSetCount=*/1, &input_images_desc_set.descriptor_set,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        });

    return static_cast<UnboundedColorPipelineOutput *>(output);
}

} // namespace e8
