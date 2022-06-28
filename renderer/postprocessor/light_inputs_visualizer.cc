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

#include "common/device.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/postprocessor/light_inputs_visualizer.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

PipelineKey const kLightInputsVisualizerPipeline = "Light Inputs Visualizer";

struct LightInputsVisualizerParameters {
    int input_to_visualize;
};

class LightInputsVisualizerPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    LightInputsVisualizerPostProcessorConfigurator(
        LightInputsRendererParameters::InputType input_to_visualize,
        PipelineOutputInterface const &light_inputs);
    ~LightInputsVisualizerPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    LightInputsRendererParameters::InputType input_to_visualize_;
    PipelineOutputInterface const &light_inputs_;
};

LightInputsVisualizerPostProcessorConfigurator::LightInputsVisualizerPostProcessorConfigurator(
    LightInputsRendererParameters::InputType input_to_visualize,
    PipelineOutputInterface const &light_inputs)
    : input_to_visualize_(input_to_visualize), light_inputs_(light_inputs) {}

LightInputsVisualizerPostProcessorConfigurator::~LightInputsVisualizerPostProcessorConfigurator() {}

void LightInputsVisualizerPostProcessorConfigurator::InputImages(
    std::vector<VkImageView> *input_images) const {
    input_images->at(LightInputsColorOutput::LICO_NORMAL_ROUGHNESS) =
        light_inputs_.ColorAttachments()[LightInputsColorOutput::LICO_NORMAL_ROUGHNESS]->view;

    input_images->at(LightInputsColorOutput::LICO_ALBEDO_METALLIC) =
        light_inputs_.ColorAttachments()[LightInputsColorOutput::LICO_ALBEDO_METALLIC]->view;
}

void LightInputsVisualizerPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> *push_constants) const {
    LightInputsVisualizerParameters *parameters =
        reinterpret_cast<LightInputsVisualizerParameters *>(push_constants->data());

    parameters->input_to_visualize = input_to_visualize_;
}

} // namespace

void DoVisualizeLightInputs(LightInputsRendererParameters::InputType input_to_visualize,
                            PipelineStage *light_inputs, DescriptorSetAllocator *desc_set_allocator,
                            VulkanContext *context, PipelineStage *target) {
    CachedPipelineInterface *pipeline = target->WithPipeline(
        kLightInputsVisualizerPipeline,
        [desc_set_allocator, context](PipelineOutputInterface *visualizer_output) {
            return std::make_unique<PostProcessorPipeline>(
                kLightInputsVisualizerPipeline, kFragmentShaderFilePathLightInputsVisualizer,
                /*input_image_count=*/LightInputsColorOutput::LightInputsColorOutputCount,
                /*push_constant_size=*/sizeof(LightInputsVisualizerParameters), visualizer_output,
                desc_set_allocator, context);
        });

    auto configurator = std::make_unique<LightInputsVisualizerPostProcessorConfigurator>(
        input_to_visualize, *light_inputs->Output());
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<PipelineStage *>{light_inputs});
}

} // namespace e8
