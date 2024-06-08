/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2023 Chifeng Wen {daviesx66@gmail.com}
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
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/shader.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/space_screen/float_map_visualizer.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kFloatMapVisualizerPipeline = "Float Map Visualizer";

struct FloatMapVisualizerParameters {
    float min_value;
    float max_value;
};

class FloatMapVisualizerUniforms : public ScreenSpaceUniformsInterface {
  public:
    FloatMapVisualizerUniforms(GraphicsPipelineOutputInterface const *float_map, float min_value,
                               float max_value)
        : float_map_(float_map), min_value_(min_value), max_value_(max_value) {}

    ~FloatMapVisualizerUniforms() = default;

    void InputImages(std::vector<VkImageView> *input_images) const override {
        input_images->at(0) = float_map_->ColorAttachments()[0]->view;
    }

    void PushConstants(std::vector<uint8_t> *push_constants) const override {
        FloatMapVisualizerParameters *parameters =
            reinterpret_cast<FloatMapVisualizerParameters *>(push_constants->data());

        parameters->min_value = min_value_;
        parameters->max_value = max_value_;
    }

  private:
    GraphicsPipelineOutputInterface const *float_map_;
    float const min_value_;
    float const max_value_;
};

std::unique_ptr<DagOperation> CreateFloatVisualizationOp(unsigned width, unsigned height,
                                                         TransferContext *transfer_context,
                                                         VulkanContext *vulkan_context) {
    auto output = std::make_shared<LdrColorOutput>(width, height, /*with_depth_buffer=*/false,
                                                   vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
}

} // namespace

DagOperationInstance DoVisualizeFloat(DagOperationInstance float_map, float min_value,
                                      float max_value, DagContext::Session *session) {
    DagOperationInstance target =
        session->WithOperation(kFloatMapVisualizerPipeline, float_map->Output()->Width(),
                               float_map->Output()->Height(), CreateFloatVisualizationOp);

    GraphicsPipelineInterface *pipeline = target->WithPipeline(
        kFloatMapVisualizerPipeline,
        [](GraphicsPipelineOutputInterface *output, VulkanContext *vulkan_context) {
            return std::make_unique<ScreenSpaceProcessorPipeline>(
                kFloatMapVisualizerPipeline, kFragmentShaderFilePathFloatMapVisualizer,
                /*input_image_count=*/1,
                /*push_constant_size=*/sizeof(FloatMapVisualizerParameters), output,
                vulkan_context);
        });

    auto configurator =
        std::make_unique<FloatMapVisualizerUniforms>(float_map->Output(), min_value, max_value);
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<DagOperationInstance>{float_map});

    return target;
}

} // namespace e8
