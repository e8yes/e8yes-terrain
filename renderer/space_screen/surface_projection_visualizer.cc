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

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/shader.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/space_screen/surface_projection_visualizer.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kSurfaceProjectionVisualizerPipeline = "Surface Projection Visualizer";

struct SurfaceProjectionVisualizerParameters {
    int parameter_to_visualize;
};

class SurfaceProjectionVisualizerPostProcessorConfigurator
    : public ScreenSpaceConfiguratorInterface {
  public:
    SurfaceProjectionVisualizerPostProcessorConfigurator(
        LightInputsRendererParameters::InputType parameter_to_visualize,
        GraphicsPipelineOutputInterface const &light_inputs);
    ~SurfaceProjectionVisualizerPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    LightInputsRendererParameters::InputType parameter_to_visualize_;
    GraphicsPipelineOutputInterface const &light_inputs_;
};

SurfaceProjectionVisualizerPostProcessorConfigurator::
    SurfaceProjectionVisualizerPostProcessorConfigurator(
        LightInputsRendererParameters::InputType parameter_to_visualize,
        GraphicsPipelineOutputInterface const &light_inputs)
    : parameter_to_visualize_(parameter_to_visualize), light_inputs_(light_inputs) {}

SurfaceProjectionVisualizerPostProcessorConfigurator::
    ~SurfaceProjectionVisualizerPostProcessorConfigurator() {}

void SurfaceProjectionVisualizerPostProcessorConfigurator::InputImages(
    std::vector<VkImageView> *input_images) const {
    input_images->at(SurfaceProjectionColorOutput::LICO_NORMAL_ROUGHNESS) =
        light_inputs_.ColorAttachments()[SurfaceProjectionColorOutput::LICO_NORMAL_ROUGHNESS]->view;

    input_images->at(SurfaceProjectionColorOutput::LICO_ALBEDO_METALLIC) =
        light_inputs_.ColorAttachments()[SurfaceProjectionColorOutput::LICO_ALBEDO_METALLIC]->view;
}

void SurfaceProjectionVisualizerPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> *push_constants) const {
    SurfaceProjectionVisualizerParameters *parameters =
        reinterpret_cast<SurfaceProjectionVisualizerParameters *>(push_constants->data());

    parameters->parameter_to_visualize = parameter_to_visualize_;
}

} // namespace

DagOperationInstance DoVisualizeSurfaceProjection(
    LightInputsRendererParameters::InputType parameter_to_visualize,
    DagOperationInstance surface_projection,
    std::shared_ptr<GraphicsPipelineOutputInterface> const &color_image_output,
    DagContext::Session *session) {
    DagOperationInstance target = session->WithOperation(
        kSurfaceProjectionVisualizerPipeline, surface_projection->Output()->Width(),
        surface_projection->Output()->Height(),
        [color_image_output](unsigned /*width*/, unsigned /*height*/,
                             TransferContext *transfer_context, VulkanContext *vulkan_context) {
            return std::make_unique<DagOperation>(color_image_output, transfer_context,
                                                  vulkan_context);
        });

    GraphicsPipelineInterface *pipeline = target->WithPipeline(
        kSurfaceProjectionVisualizerPipeline,
        [](GraphicsPipelineOutputInterface *visualizer_output, TransferContext *transfer_context,
           VulkanContext *vulkan_context) {
            return std::make_unique<ScreenSpaceProcessorPipeline>(
                kSurfaceProjectionVisualizerPipeline, kFragmentShaderFilePathLightInputsVisualizer,
                /*input_image_count=*/SurfaceProjectionColorOutput::LightInputsColorOutputCount,
                /*push_constant_size=*/sizeof(SurfaceProjectionVisualizerParameters),
                visualizer_output, transfer_context, vulkan_context);
        });

    auto configurator = std::make_unique<SurfaceProjectionVisualizerPostProcessorConfigurator>(
        parameter_to_visualize, *surface_projection->Output());
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<DagOperation *>{surface_projection});

    return target;
}

} // namespace e8
