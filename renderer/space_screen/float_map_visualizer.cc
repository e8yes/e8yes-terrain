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
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/space_screen/float_map_visualizer.h"
#include "renderer/space_screen/post_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kFloatMapVisualizerPipeline = "Float Map Visualizer";

struct FloatMapVisualizerParameters {
    float min_value;
    float max_value;
};

class FloatMapVisualizerConfigurator : public PostProcessorConfiguratorInterface {
  public:
    FloatMapVisualizerConfigurator(GraphicsPipelineOutputInterface const &float_map, float min_value,
                                   float max_value)
        : float_map_(float_map), min_value_(min_value), max_value_(max_value) {}

    ~FloatMapVisualizerConfigurator() = default;

    void InputImages(std::vector<VkImageView> *input_images) const override {
        input_images->at(0) = float_map_.ColorAttachments()[0]->view;
    }

    void PushConstants(std::vector<uint8_t> *push_constants) const override {
        FloatMapVisualizerParameters *parameters =
            reinterpret_cast<FloatMapVisualizerParameters *>(push_constants->data());

        parameters->min_value = min_value_;
        parameters->max_value = max_value_;
    }

  private:
    GraphicsPipelineOutputInterface const &float_map_;
    float const min_value_;
    float const max_value_;
};

} // namespace

void DoVisualizeFloat(DagOperation *float_map_stage, float min_value, float max_value,
                      TransferContext *transfer_context, DagOperation *target) {
    GraphicsPipelineInterface *pipeline = target->WithPipeline(
        kFloatMapVisualizerPipeline, [transfer_context](GraphicsPipelineOutputInterface *output) {
            return std::make_unique<PostProcessorPipeline>(
                kFloatMapVisualizerPipeline, kFragmentShaderFilePathFloatMapVisualizer,
                /*input_image_count=*/1,
                /*push_constant_size=*/sizeof(FloatMapVisualizerParameters), output,
                transfer_context);
        });

    auto configurator = std::make_unique<FloatMapVisualizerConfigurator>(*float_map_stage->Output(),
                                                                         min_value, max_value);
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<DagOperation *>{float_map_stage});
}

} // namespace e8
