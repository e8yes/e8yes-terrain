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

#ifndef ISLANDS_RENDERER_CACHED_PIPELINE_H
#define ISLANDS_RENDERER_CACHED_PIPELINE_H

#include <memory>
#include <string>
#include <vector>

#include "common/device.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"

namespace e8 {

// The unique key assigned to a cached pipeline.
using PipelineKey = std::string;

/**
 * @brief The CachedPipelineArgumentsInterface struct Generic arguments that pass to
 * CachedPipelineInterface::Launch().
 */
struct CachedPipelineArgumentsInterface {
    CachedPipelineArgumentsInterface();
    virtual ~CachedPipelineArgumentsInterface();
};

/**
 * @brief The CachedPipelineInterface class Encapsulates the common states and functionality of a
 * graphics pipeline.
 */
class CachedPipelineInterface {
  public:
    CachedPipelineInterface(VulkanContext *context);
    virtual ~CachedPipelineInterface();

    /**
     * @brief Key A unique key assigned to this pipeline.
     */
    virtual PipelineKey Key() const = 0;

    /**
     * @brief Launch Launches a graphics pipeline, and let it run asynchronously on the GPU.
     *
     * @param generic_args Arguments passed to this launch.
     * @param prerequisites The set of promises that need to be resolved before running this
     * pipeline.
     * @param completion_signal_count The number of signals to emit when the pipeline is complete.
     * @param output Stores the output of this pipeline.
     * @return The GPU operation's promised fulfillment.
     */
    virtual Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                               std::vector<GpuPromise *> const &prerequisites,
                               unsigned completion_signal_count,
                               PipelineOutputInterface *output) = 0;

  protected:
    // Contextual Vulkan handles.
    VulkanContext *context_;

    // Common pipeline states which are usually populated on construction.
    std::unique_ptr<ShaderStages> shader_stages_;
    std::unique_ptr<ShaderUniformLayout> uniform_layout_;
    std::unique_ptr<VertexInputInfo> vertex_inputs_;
    std::unique_ptr<FixedStageConfig> fixed_stage_config_;
    std::unique_ptr<GraphicsPipeline> pipeline_;
    std::unique_ptr<ImageSampler> texture_sampler_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_CACHED_PIPELINE_H
