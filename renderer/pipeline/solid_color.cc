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

#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/pass/rasterize.h"
#include "renderer/pipeline/solid_color.h"

namespace e8 {
namespace {

PipelineStage::PipelineKey const kFillColorPipeline = "FillColor";

class FillColorPipeline : public CachedPipelineInterface {
  public:
    FillColorPipeline(VulkanContext *context);
    ~FillColorPipeline() override;

    Fulfillment Launch(unsigned instance_id, std::vector<PipelineOutputInterface *> const &inputs,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;

    void SetFillColor(vec3 const &color);

  private:
    vec3 fill_color_;
};

FillColorPipeline::FillColorPipeline(VulkanContext *context) : CachedPipelineInterface(context) {}

FillColorPipeline::~FillColorPipeline() {}

Fulfillment FillColorPipeline::Launch(unsigned /*instance_id*/,
                                      std::vector<PipelineOutputInterface *> const & /*inputs*/,
                                      std::vector<GpuPromise *> const &prerequisites,
                                      unsigned completion_signal_count,
                                      PipelineOutputInterface *output) {

    FrameBuffer *frame_buffer = output->GetFrameBuffer();

    frame_buffer->clear_values[0].color.float32[0] = fill_color_(0);
    frame_buffer->clear_values[0].color.float32[1] = fill_color_(1);
    frame_buffer->clear_values[0].color.float32[2] = fill_color_(2);

    VkCommandBuffer cmds = StartRenderPass(output->GetRenderPass(), *frame_buffer, context_);
    return FinishRenderPass2(cmds, completion_signal_count, prerequisites, context_);
}

void FillColorPipeline::SetFillColor(vec3 const &color) { fill_color_ = color; }

} // namespace

SolidColorPipeline::SolidColorPipeline(VulkanContext *context) : context_(context) {}

SolidColorPipeline::~SolidColorPipeline() {}

void SolidColorPipeline::Run(vec3 const &color, GpuPromise const &prerequisites,
                             PipelineOutputInterface *output) {
    FrameBuffer *frame_buffer = output->GetFrameBuffer();

    frame_buffer->clear_values[0].color.float32[0] = color(0);
    frame_buffer->clear_values[0].color.float32[1] = color(1);
    frame_buffer->clear_values[0].color.float32[2] = color(2);

    VkCommandBuffer cmds = StartRenderPass(output->GetRenderPass(), *frame_buffer, context_);
    RenderPassPromise promise = FinishRenderPass(cmds, prerequisites, context_);
    output->AddWriter(std::move(promise.gpu), std::move(promise.cpu));
}

void DoFillColor(vec3 const &color, VulkanContext *context,
                 PipelineStage *first_stage, PipelineStage *target) {
    FillColorPipeline *pipeline = static_cast<FillColorPipeline *>(
        target->WithPipeline(kFillColorPipeline, [context](PipelineOutputInterface * /*output*/) {
            return std::make_unique<FillColorPipeline>(context);
        }));
    pipeline->SetFillColor(color);

    target->Schedule(kFillColorPipeline, /*parents=*/std::vector<PipelineStage*>{first_stage},
                     /*instance_count=*/1);
}

} // namespace e8
