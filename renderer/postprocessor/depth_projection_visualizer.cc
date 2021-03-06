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

#include <vulkan/vulkan.h>
#include <memory>
#include <optional>
#include <vector>

#include "common/device.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/postprocessor/depth_projection_visualizer.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey kDepthProjectionVisualizerPipeline = "Depth Projection Visualizer";

struct DepthProjectionVisualizerParameters {
    float z_near;
    float z_far;
    float alpha;
};

class DepthProjectionPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
   public:
    DepthProjectionPostProcessorConfigurator(float alpha,
                                             std::optional<PerspectiveProjection> projection,
                                             PipelineOutputInterface const &depth_map);
    ~DepthProjectionPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

   private:
    float const alpha_;
    std::optional<PerspectiveProjection> const projection_;
    PipelineOutputInterface const &depth_map_;
};

DepthProjectionPostProcessorConfigurator::DepthProjectionPostProcessorConfigurator(
    float alpha, std::optional<PerspectiveProjection> projection,
    PipelineOutputInterface const &depth_map)
    : alpha_(alpha), projection_(projection), depth_map_(depth_map) {}

DepthProjectionPostProcessorConfigurator::~DepthProjectionPostProcessorConfigurator() {}

void DepthProjectionPostProcessorConfigurator::InputImages(
    std::vector<VkImageView> *input_images) const {
    input_images->at(0) = depth_map_.DepthAttachment()->view;
}

void DepthProjectionPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> *push_constants) const {
    DepthProjectionVisualizerParameters *parameters =
        reinterpret_cast<DepthProjectionVisualizerParameters *>(push_constants->data());

    parameters->alpha = alpha_;

    if (projection_.has_value()) {
        parameters->z_near = projection_->Frustum().z_near;
        parameters->z_far = projection_->Frustum().z_far;
    } else {
        parameters->z_near = 1.0f;
        parameters->z_far = 100.0f;
    }
}

}  // namespace

void DoVisualizeDepthProjection(float alpha, std::optional<PerspectiveProjection> projection,
                                PipelineStage *depth_map_stage, TransferContext *transfer_context,
                                PipelineStage *target) {
    CachedPipelineInterface *pipeline = target->WithPipeline(
        kDepthProjectionVisualizerPipeline, [transfer_context](PipelineOutputInterface *output) {
            return std::make_unique<PostProcessorPipeline>(
                kDepthProjectionVisualizerPipeline, kFragmentShaderFilePathDepthMapVisualizer,
                /*input_image_count=*/1,
                /*push_constant_size=*/sizeof(DepthProjectionVisualizerParameters), output,
                transfer_context);
        });

    auto configurator = std::make_unique<DepthProjectionPostProcessorConfigurator>(
        alpha, projection, *depth_map_stage->Output());
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<PipelineStage *>{depth_map_stage});
}

}  // namespace e8
