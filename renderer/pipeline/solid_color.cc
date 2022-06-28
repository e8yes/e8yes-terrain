/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * <p>You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "renderer/pipeline/solid_color.h"

#include <vulkan/vulkan.h>

#include <vector>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/pass/rasterize.h"

namespace e8 {
namespace {

PipelineKey const kFillColorPipeline = "FillColor";

struct FillColorPipelineArguments : public CachedPipelineArgumentsInterface {
    FillColorPipelineArguments(vec3 const &color) : color(color) {}

    vec3 color;
};

class FillColorPipeline : public CachedPipelineInterface {
  public:
    FillColorPipeline(VulkanContext *context);
    ~FillColorPipeline() override;

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;
};

FillColorPipeline::FillColorPipeline(VulkanContext *context) : CachedPipelineInterface(context) {}

FillColorPipeline::~FillColorPipeline() {}

PipelineKey FillColorPipeline::Key() const { return kFillColorPipeline; }

Fulfillment FillColorPipeline::Launch(CachedPipelineArgumentsInterface const &generic_args,
                                      std::vector<GpuPromise *> const &prerequisites,
                                      unsigned completion_signal_count,
                                      PipelineOutputInterface *output) {
    FillColorPipelineArguments const &args =
        static_cast<FillColorPipelineArguments const &>(generic_args);

    FrameBuffer *frame_buffer = output->GetFrameBuffer();

    frame_buffer->clear_values[0].color.float32[0] = args.color(0);
    frame_buffer->clear_values[0].color.float32[1] = args.color(1);
    frame_buffer->clear_values[0].color.float32[2] = args.color(2);

    VkCommandBuffer cmds = StartRenderPass(output->GetRenderPass(), *frame_buffer, context_);
    return FinishRenderPass(cmds, completion_signal_count, prerequisites, context_);
}

} // namespace

void DoFillColor(vec3 const &color, VulkanContext *context, PipelineStage *first_stage,
                 PipelineStage *target) {
    CachedPipelineInterface *pipeline =
        target->WithPipeline(kFillColorPipeline, [context](PipelineOutputInterface * /*output*/) {
            return std::make_unique<FillColorPipeline>(context);
        });

    auto args = std::make_unique<FillColorPipelineArguments>(color);
    target->Schedule(pipeline, std::move(args),
                     /*parents=*/std::vector<PipelineStage *>{first_stage});
}

} // namespace e8
