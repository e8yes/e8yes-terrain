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

#include "common/device.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/postprocessor/tone_map.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

std::vector<VkDescriptorSetLayoutBinding> RadianceInputBinding() {
    VkDescriptorSetLayoutBinding radiance_binding{};
    radiance_binding.binding = 0;
    radiance_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    radiance_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    radiance_binding.descriptorCount = 1;

    return std::vector<VkDescriptorSetLayoutBinding>{radiance_binding};
}

} // namespace

struct ToneMapPipeline::ToneMapPipelineImpl {
    ToneMapPipelineImpl(PipelineOutputInterface *color_output,
                        DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~ToneMapPipelineImpl();

    VulkanContext *context;
    DescriptorSetAllocator *desc_set_allocator;
    std::unique_ptr<ImageSampler> radiance_sampler;
    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;

    UnboundedColorPipelineOutput const *current_radiance;
};

ToneMapPipeline::ToneMapPipelineImpl::ToneMapPipelineImpl(
    PipelineOutputInterface *color_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : context(context) {
    radiance_sampler = CreateReadBackSampler(context);

    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathHdrClamp, /*push_constant=*/std::nullopt, RadianceInputBinding(),
        color_output, desc_set_allocator, context);
}

ToneMapPipeline::ToneMapPipelineImpl::~ToneMapPipelineImpl() {}

ToneMapPipeline::ToneMapPipeline(PipelineOutputInterface *color_output,
                                 DescriptorSetAllocator *desc_set_allocator, VulkanContext *context)
    : pimpl_(std::make_unique<ToneMapPipelineImpl>(color_output, desc_set_allocator, context)) {}

ToneMapPipeline::~ToneMapPipeline() {}

PipelineOutputInterface *ToneMapPipeline::Run(UnboundedColorPipelineOutput const &radiance) {
    return pimpl_->post_processor_pipeline->Run(
        *radiance.promise, /*set_uniforms_fn=*/
        [this, &radiance](ShaderUniformLayout const &uniform_layout,
                          DescriptorSet const &input_images_desc_set, VkCommandBuffer cmds) {
            if (&radiance != pimpl_->current_radiance) {
                // Sets the radiance map.
                WriteImageDescriptor(radiance.ColorAttachments()[0]->view,
                                     *pimpl_->radiance_sampler, input_images_desc_set,
                                     /*binding=*/0, pimpl_->context);

                pimpl_->current_radiance = &radiance;
            }

            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                    /*firstSet=*/DescriptorSetType::DST_PER_PASS,
                                    /*descriptorSetCount=*/1, &input_images_desc_set.descriptor_set,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        });
}

} // namespace e8
