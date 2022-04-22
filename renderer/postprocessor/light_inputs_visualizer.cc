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

struct LightInputsVisualizerParameters {
    int input_to_visualize;
};

class LightInputsVisualizerPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    LightInputsVisualizerPostProcessorConfigurator(
        LightInputsRendererParameters::InputType input_to_visualize,
        LightInputsPipelineOutput const &light_inputs);
    ~LightInputsVisualizerPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    LightInputsRendererParameters::InputType input_to_visualize_;
    LightInputsPipelineOutput const &light_inputs_;
};

LightInputsVisualizerPostProcessorConfigurator::LightInputsVisualizerPostProcessorConfigurator(
    LightInputsRendererParameters::InputType input_to_visualize,
    LightInputsPipelineOutput const &light_inputs)
    : input_to_visualize_(input_to_visualize), light_inputs_(light_inputs) {}

LightInputsVisualizerPostProcessorConfigurator::~LightInputsVisualizerPostProcessorConfigurator() {}

void LightInputsVisualizerPostProcessorConfigurator::InputImages(
    std::vector<VkImageView> *input_images) const {
    input_images->at(LightInputsPipelineOutput::NORMAL_ROUGHNESS) =
        light_inputs_.ColorAttachments()[LightInputsPipelineOutput::NORMAL_ROUGHNESS]->view;

    input_images->at(LightInputsPipelineOutput::ALBEDO_METALLIC) =
        light_inputs_.ColorAttachments()[LightInputsPipelineOutput::ALBEDO_METALLIC]->view;
}

void LightInputsVisualizerPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> *push_constants) const {
    LightInputsVisualizerParameters *parameters =
        reinterpret_cast<LightInputsVisualizerParameters *>(push_constants->data());

    parameters->input_to_visualize = input_to_visualize_;
}

} // namespace

struct LightInputsVisualizerPipeline::LightInputsVisualizerPipelineImpl {
    LightInputsVisualizerPipelineImpl(PipelineOutputInterface *visualizer_output,
                                      DescriptorSetAllocator *desc_set_allocator,
                                      VulkanContext *context);
    ~LightInputsVisualizerPipelineImpl();

    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;
};

LightInputsVisualizerPipeline::LightInputsVisualizerPipelineImpl::LightInputsVisualizerPipelineImpl(
    PipelineOutputInterface *visualizer_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context) {
    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathLightInputsVisualizer,
        /*input_image_count=*/LightInputsPipelineOutput::ColorOutputCount,
        /*push_constant_size=*/sizeof(LightInputsVisualizerParameters), visualizer_output,
        desc_set_allocator, context);
}

LightInputsVisualizerPipeline::LightInputsVisualizerPipelineImpl::
    ~LightInputsVisualizerPipelineImpl() {}

LightInputsVisualizerPipeline::LightInputsVisualizerPipeline(
    DescriptorSetAllocator *desc_set_allocator, VulkanContext *context)
    : desc_set_allocator_(desc_set_allocator), context_(context), current_output_(nullptr) {}

LightInputsVisualizerPipeline::~LightInputsVisualizerPipeline() {}

void LightInputsVisualizerPipeline::Run(LightInputsRendererParameters::InputType input_to_visualize,
                                        LightInputsPipelineOutput const &light_inputs,
                                        PipelineOutputInterface *output) {
    if (output != current_output_) {
        pimpl_ = std::make_unique<LightInputsVisualizerPipelineImpl>(output, desc_set_allocator_,
                                                                     context_);
        current_output_ = output;
    }

    LightInputsVisualizerPostProcessorConfigurator configurator(input_to_visualize, light_inputs);
    pimpl_->post_processor_pipeline->Run(configurator, *light_inputs.Promise());
}

} // namespace e8
