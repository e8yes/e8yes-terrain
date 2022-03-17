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
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_output.h"
#include "renderer/pipeline_post_processor.h"
#include "renderer/render_pass.h"

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
UniformLayout(std::optional<VkPushConstantRange> const &push_constant,
              std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
              VulkanContext *context) {
    return CreateShaderUniformLayout(
        push_constant,
        /*per_frame_desc_set=*/
        std::vector<VkDescriptorSetLayoutBinding>{PostProcessorViewportBinding()},
        /*per_pass_desc_set=*/per_pass_desc_set,
        /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
}

} // namespace

struct PostProcessorPipeline::PostProcessorPipelineImpl {
    PostProcessorPipelineImpl(std::string const &fragment_shader,
                              std::optional<VkPushConstantRange> const &push_constant,
                              std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
                              PipelineOutputInterface *output, VulkanContext *context);
    ~PostProcessorPipelineImpl();

    PipelineOutputInterface *output;
    VulkanContext *context;

    std::unique_ptr<ShaderStages> shader_stages;
    std::unique_ptr<ShaderUniformLayout> uniform_layout;
    std::unique_ptr<DescriptorSets> descriptor_sets;
    std::unique_ptr<VertexInputInfo> vertex_inputs;
    std::unique_ptr<FixedStageConfig> fixed_stage_config;

    std::unique_ptr<UniformBuffer> viewport_dimension_ubo;

    std::unique_ptr<GraphicsPipeline> pipeline;
};

PostProcessorPipeline::PostProcessorPipelineImpl::PostProcessorPipelineImpl(
    std::string const &fragment_shader, std::optional<VkPushConstantRange> const &push_constant,
    std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
    PipelineOutputInterface *output, VulkanContext *context)
    : output(output), context(context) {
    shader_stages =
        CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathPostProcessor,
                           /*fragment_shader_file_path=*/fragment_shader, context);
    uniform_layout = UniformLayout(push_constant, per_pass_desc_set, context);
    descriptor_sets = CreateDescriptorSets(*uniform_layout, context);
    vertex_inputs = CreateVertexInputState(
        /*input_attributes=*/std::vector<VkVertexInputAttributeDescription>());
    fixed_stage_config =
        CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                               /*cull_mode=*/VK_CULL_MODE_NONE,
                               /*enable_depth_test=*/false, output->width, output->height);

    viewport_dimension_ubo = CreateUniformBuffer(/*size=*/sizeof(ViewportDimension), context);

    pipeline =
        CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages, *this->uniform_layout,
                               *vertex_inputs, *fixed_stage_config, context);
}

PostProcessorPipeline::PostProcessorPipelineImpl::~PostProcessorPipelineImpl() {}

PostProcessorPipeline::PostProcessorPipeline(
    std::string const &fragment_shader, std::optional<VkPushConstantRange> const &push_constant,
    std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
    PipelineOutputInterface *output, VulkanContext *context)
    : pimpl_(std::make_unique<PostProcessorPipelineImpl>(fragment_shader, push_constant,
                                                         per_pass_desc_set, output, context)) {}

PostProcessorPipeline::PostProcessorPipeline(PipelineOutputInterface *output,
                                             VulkanContext *context)
    : PostProcessorPipeline(kFragmentShaderFilePathPostProcessorEmpty,
                            /*push_constant=*/std::nullopt,
                            /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
                            output, context) {}

PostProcessorPipeline::~PostProcessorPipeline() {}

PipelineOutputInterface *
PostProcessorPipeline::Run(GpuBarrier const &barrier,
                           SetPostProcessorUniformsExFn const &set_uniforms_fn) {
    ViewportDimension dimension;
    dimension.viewport_width = pimpl_->output->width;
    dimension.viewport_height = pimpl_->output->height;

    WriteUniformBufferDescriptor(&dimension, *pimpl_->viewport_dimension_ubo,
                                 pimpl_->descriptor_sets->frame,
                                 /*binding=*/0, pimpl_->context);

    VkCommandBuffer cmds = StartRenderPass(pimpl_->output->GetRenderPass(),
                                           *pimpl_->output->GetFrameBuffer(), pimpl_->context);

    PostProcess(
        *pimpl_->pipeline, *pimpl_->uniform_layout,
        [this, set_uniforms_fn](ShaderUniformLayout const &uniform_layout, VkCommandBuffer cmds) {
            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pimpl_->uniform_layout->layout, /*firstSet=*/0,
                                    /*descriptorSetCount=*/1, &pimpl_->descriptor_sets->frame,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);

            if (set_uniforms_fn != nullptr) {
                set_uniforms_fn(uniform_layout, pimpl_->descriptor_sets->pass, cmds);
            }
        },
        cmds);

    pimpl_->output->barrier =
        FinishRenderPass(cmds, barrier, pimpl_->output->AcquireFence(), pimpl_->context);

    return pimpl_->output;
}

} // namespace e8
