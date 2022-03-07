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

void NoOpSetPostProcessorUniformsFn(ShaderUniformLayout const &, VkCommandBuffer) {}

std::unique_ptr<ShaderUniformLayout> EmptyShaderUniformLayout(VulkanContext *context) {
    return CreateShaderUniformLayout(
        /*push_constant=*/std::nullopt,
        /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
}

} // namespace

struct PostProcessorPipeline::PostProcessorPipelineImpl {
    PostProcessorPipelineImpl(std::string const &fragment_shader,
                              std::unique_ptr<ShaderUniformLayout> uniform_layout,
                              SetPostProcessorUniformsFn const &set_uniforms_fn,
                              PipelineOutputInterface *output, VulkanContext *context);
    ~PostProcessorPipelineImpl();

    SetPostProcessorUniformsFn set_uniforms_fn;

    PipelineOutputInterface *output;
    VulkanContext *context;

    std::unique_ptr<ShaderStages> shader_stages;
    std::unique_ptr<ShaderUniformLayout> uniform_layout;
    std::unique_ptr<VertexInputInfo> vertex_inputs;
    std::unique_ptr<FixedStageConfig> fixed_stage_config;

    std::unique_ptr<GraphicsPipeline> pipeline;
};

PostProcessorPipeline::PostProcessorPipelineImpl::PostProcessorPipelineImpl(
    std::string const &fragment_shader, std::unique_ptr<ShaderUniformLayout> uniform_layout,
    SetPostProcessorUniformsFn const &set_uniforms_fn, PipelineOutputInterface *output,
    VulkanContext *context)
    : set_uniforms_fn(set_uniforms_fn), output(output), context(context),
      uniform_layout(std::move(uniform_layout)) {
    shader_stages =
        CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathPostProcessor,
                           /*fragment_shader_file_path=*/fragment_shader, context);
    vertex_inputs = CreateVertexInputState(
        /*input_attributes=*/std::vector<VkVertexInputAttributeDescription>());
    fixed_stage_config =
        CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                               /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                               /*enable_depth_test=*/false, output->width, output->height);

    pipeline =
        CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages, *this->uniform_layout,
                               *vertex_inputs, *fixed_stage_config, context);
}

PostProcessorPipeline::PostProcessorPipelineImpl::~PostProcessorPipelineImpl() {}

PostProcessorPipeline::PostProcessorPipeline(std::string const &fragment_shader,
                                             std::unique_ptr<ShaderUniformLayout> uniform_layout,
                                             SetPostProcessorUniformsFn const &set_uniforms_fn,
                                             PipelineOutputInterface *output,
                                             VulkanContext *context)
    : pimpl_(std::make_unique<PostProcessorPipelineImpl>(fragment_shader, std::move(uniform_layout),
                                                         set_uniforms_fn, output, context)) {}

PostProcessorPipeline::PostProcessorPipeline(PipelineOutputInterface *output,
                                             VulkanContext *context)
    : PostProcessorPipeline(kFragmentShaderFilePathPostProcessorEmpty,
                            EmptyShaderUniformLayout(context), NoOpSetPostProcessorUniformsFn,
                            output, context) {}

PostProcessorPipeline::~PostProcessorPipeline() {}

PipelineOutputInterface *PostProcessorPipeline::Run(GpuBarrier const &barrier) {
    VkCommandBuffer cmds = StartRenderPass(pimpl_->output->GetRenderPass(),
                                           *pimpl_->output->GetFrameBuffer(), pimpl_->context);

    PostProcess(*pimpl_->pipeline, *pimpl_->uniform_layout, pimpl_->set_uniforms_fn, cmds);

    pimpl_->output->barrier =
        FinishRenderPass(cmds, barrier, pimpl_->output->FinalOutput(), pimpl_->context);

    return pimpl_->output;
}

} // namespace e8
