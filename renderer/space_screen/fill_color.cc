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

#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/space_screen/fill_color.h"

namespace e8 {
namespace {

PipelineKey const kFillColorPipeline = "FillColor";

struct FillColorArguments : public GraphicsPipelineArgumentsInterface {
    FillColorArguments(vec3 const &color) : color(color) {}

    vec3 color;
};

class FillColorPipeline : public GraphicsPipelineInterface {
  public:
    FillColorPipeline(VulkanContext *context) : GraphicsPipelineInterface(context) {}
    ~FillColorPipeline() override = default;

    PipelineKey Key() const override { return kFillColorPipeline; }

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *output, CommandBuffer *command_buffer) override {
        FillColorArguments const &args = static_cast<FillColorArguments const &>(generic_args);

        FrameBuffer *frame_buffer = output->GetFrameBuffer();

        frame_buffer->clear_values[0].color.float32[0] = args.color(0);
        frame_buffer->clear_values[0].color.float32[1] = args.color(1);
        frame_buffer->clear_values[0].color.float32[2] = args.color(2);

        StartRenderPass(output->GetRenderPass(), *frame_buffer, command_buffer);
        FinishRenderPass(command_buffer);
    }
};

std::unique_ptr<DagOperation> CreateFillColorOp(unsigned width, unsigned height, bool hdr,
                                                VulkanContext *context) {
    std::shared_ptr<GraphicsPipelineOutputInterface> output;
    if (hdr) {
        output =
            std::make_shared<HdrColorOutput>(width, height, /*with_depth_buffer=*/false, context);
    } else {
        output =
            std::make_shared<LdrColorOutput>(width, height, /*with_depth_buffer=*/false, context);
    }

    return std::make_unique<DagOperation>(output);
}

void ConfigureFillColorOp(vec3 const &color, DagOperationInstance dependent_op,
                          VulkanContext *context, DagOperationInstance filled_image) {
    GraphicsPipelineInterface *pipeline = filled_image->WithPipeline(
        kFillColorPipeline, [context](GraphicsPipelineOutputInterface * /*output*/) {
            return std::make_unique<FillColorPipeline>(context);
        });
    auto args = std::make_unique<FillColorArguments>(color);
    filled_image->Schedule(pipeline, std::move(args),
                           /*parents=*/std::vector<DagOperation *>{dependent_op});
}

} // namespace

DagOperationInstance
DoFillColor(vec3 const &color,
            std::shared_ptr<GraphicsPipelineOutputInterface> const &color_image_output,
            VulkanContext *context, DagContext *dag) {
    DagContext::DagOperationKey key = CreateDagOperationKey(
        kFillColorPipeline, color_image_output->Width(), color_image_output->Height());
    DagOperationInstance filled_image =
        dag->WithOperation(key, [color_image_output](VulkanContext *) {
            return std::make_unique<DagOperation>(color_image_output);
        });

    ConfigureFillColorOp(color, /*dependent_op=*/nullptr, context, filled_image);
    return filled_image;
}

DagOperationInstance DoFillColor(vec3 const &color, bool hdr, DagOperationInstance image,
                                 VulkanContext *context, DagContext *dag) {
    assert(image != nullptr);

    DagContext::DagOperationKey key = CreateDagOperationKey(
        kFillColorPipeline, image->Output()->Width(), image->Output()->Height());
    DagOperationInstance filled_image = dag->WithOperation(key, [hdr,
                                                                 image](VulkanContext *context) {
        return CreateFillColorOp(image->Output()->Width(), image->Output()->Height(), hdr, context);
    });

    ConfigureFillColorOp(color, image, context, filled_image);
    return filled_image;
}

} // namespace e8
