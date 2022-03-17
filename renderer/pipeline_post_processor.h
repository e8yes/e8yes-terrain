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

#ifndef ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H
#define ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H

#include <memory>
#include <string>

#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_output.h"
#include "renderer/render_pass.h"

namespace e8 {

/**
 * @brief The PostProcessorPipeline class It offers a basic framework for setting up a post
 * processing graphics pipeline. Supplies proper arguments to the constructor of this class to
 * define the actual effect.
 */
class PostProcessorPipeline {
  public:
    //
    using SetPostProcessorUniformsExFn = std::function<void(
        ShaderUniformLayout const &uniform_layout, VkDescriptorSet per_pass, VkCommandBuffer cmds)>;

    /**
     * @brief PostProcessorPipeline Constructs a custom post processor.
     *
     * @param fragment_shader The fragment shader to create the desired post processing effect.
     * @param push_constant
     * @param per_pass_desc_set
     * @param output To receive output from the post processor.
     * @param context Contextual Vulkan handles.
     */
    PostProcessorPipeline(std::string const &fragment_shader,
                          std::optional<VkPushConstantRange> const &push_constant,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
                          PipelineOutputInterface *output, VulkanContext *context);

    /**
     * @brief PostProcessorPipeline Constructs an empty post processor. It loads the empty post
     * processor shader and outputs the UV coordinate as color.
     *
     * @param output To receive output from the post processor.
     * @param context Contextual Vulkan handles.
     */
    PostProcessorPipeline(PipelineOutputInterface *output, VulkanContext *context);

    ~PostProcessorPipeline();

    /**
     * @brief Run Runs the post processing graphics pipeline. The pipeline can only be run when the
     * previous run was finished (indicated by the output's barrier).
     *
     * @param barrier The previous tasks' barrier.
     * @param set_uniforms_fn A function to set the values of the shader's uniform variables. If not
     * supplied, an empty function will be used.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(GpuBarrier const &barrier,
                                 SetPostProcessorUniformsExFn const &set_uniforms_fn = nullptr);

  private:
    struct PostProcessorPipelineImpl;
    std::unique_ptr<PostProcessorPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H
