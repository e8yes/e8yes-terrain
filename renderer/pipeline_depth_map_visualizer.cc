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
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_depth_map_visualizer.h"
#include "renderer/pipeline_output.h"
#include "renderer/pipeline_post_processor.h"

namespace e8 {
namespace {

struct PushConstants {
    PushConstants();
    ~PushConstants();

    float z_near;
    float z_far;
    float alpha;
};

PushConstants::PushConstants() : z_near(1), z_far(10), alpha(0.0f) {}

PushConstants::~PushConstants() {}

VkPushConstantRange PushConstantRange() {
    VkPushConstantRange push_constant{};

    push_constant.offset = 0;
    push_constant.size = sizeof(PushConstants);
    push_constant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    return push_constant;
}

std::vector<VkDescriptorSetLayoutBinding> DepthMapBinding() {
    VkDescriptorSetLayoutBinding depth_map_binding{};

    depth_map_binding.binding = 0;
    depth_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    depth_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    depth_map_binding.descriptorCount = 1;

    return std::vector<VkDescriptorSetLayoutBinding>{depth_map_binding};
}

} // namespace

struct DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl {
    DepthMapVisualizerPipelineImpl(PipelineOutputInterface *visualizer_output,
                                   VulkanContext *context);
    ~DepthMapVisualizerPipelineImpl();

    VulkanContext *context;

    std::unique_ptr<ImageSampler> depth_map_sampler;
    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;
};

DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl::DepthMapVisualizerPipelineImpl(
    PipelineOutputInterface *visualizer_output, VulkanContext *context)
    : context(context) {
    depth_map_sampler = CreateReadBackSampler(context);

    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathDepthMapVisualizer, PushConstantRange(), DepthMapBinding(),
        visualizer_output, context);
}

DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl::~DepthMapVisualizerPipelineImpl() {}

DepthMapVisualizerPipeline::DepthMapVisualizerPipeline(PipelineOutputInterface *visualizer_output,
                                                       VulkanContext *context)
    : pimpl_(std::make_unique<DepthMapVisualizerPipelineImpl>(visualizer_output, context)) {}

DepthMapVisualizerPipeline::~DepthMapVisualizerPipeline() {}

PipelineOutputInterface *
DepthMapVisualizerPipeline::Run(float alpha, std::optional<PerspectiveProjection> projection,
                                PipelineOutputInterface const &depth_map) {
    return pimpl_->post_processor_pipeline->Run(
        *depth_map.barrier, /*set_uniforms_fn=*/
        [this, alpha, projection, &depth_map](ShaderUniformLayout const &uniform_layout,
                                              VkDescriptorSet per_pass, VkCommandBuffer cmds) {
            // Sets projection and visualizer parameters.
            PushConstants push_constants;
            push_constants.alpha = alpha;
            if (projection.has_value()) {
                push_constants.z_near = projection->Frustum().z_near;
                push_constants.z_far = projection->Frustum().z_far;
            }

            vkCmdPushConstants(cmds, uniform_layout.layout,
                               /*stageFlags=*/VK_SHADER_STAGE_FRAGMENT_BIT,
                               /*offset=*/0,
                               /*size=*/sizeof(PushConstants), &push_constants);

            // Sets the depth map input.
            WriteImageDescriptor(
                depth_map.DepthAttachment()->view, depth_map.DepthAttachment()->desc.finalLayout,
                *pimpl_->depth_map_sampler, per_pass, /*binding=*/0, pimpl_->context);

            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                    /*firstSet=*/1,
                                    /*descriptorSetCount=*/1, &per_pass,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        });
}

} // namespace e8
