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
#include <vector>

#include "common/device.h"
#include "common/tensor.h"
#include "content/proto/light_source.pb.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/postprocessor/radiance_directional.h"
#include "renderer/transfer/descriptor_set.h"
#include "resource/common.h"

namespace e8 {
namespace {

struct SunLightParameters {
    vec4 dir;
    vec4 intensity;
};

class SunLightPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    SunLightPostProcessorConfigurator(SunLight const &sun_light,
                                      LightInputsPipelineOutput const &light_inputs);
    ~SunLightPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SunLight const &sun_light_;
    LightInputsPipelineOutput const &light_inputs_;
};

SunLightPostProcessorConfigurator::SunLightPostProcessorConfigurator(
    SunLight const &sun_light, LightInputsPipelineOutput const &light_inputs)
    : sun_light_(sun_light), light_inputs_(light_inputs) {}

SunLightPostProcessorConfigurator::~SunLightPostProcessorConfigurator() {}

void SunLightPostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(LightInputsPipelineOutput::NORMAL_ROUGHNESS) =
        light_inputs_.ColorAttachments()[LightInputsPipelineOutput::NORMAL_ROUGHNESS]->view;

    input_images->at(LightInputsPipelineOutput::ALBEDO_METALLIC) =
        light_inputs_.ColorAttachments()[LightInputsPipelineOutput::ALBEDO_METALLIC]->view;
}

void SunLightPostProcessorConfigurator::PushConstants(std::vector<uint8_t> *push_constants) const {
    SunLightParameters *parameters = reinterpret_cast<SunLightParameters *>(push_constants->data());

    parameters->dir = ToVec3(sun_light_.direction()).homo(0.0f);
    parameters->intensity = ToVec3(sun_light_.intensity()).homo(1.0f);
}

} // namespace

struct DirectionalRadiancePipeline::DirectionalRadiancePipelineImpl {
    DirectionalRadiancePipelineImpl(UnboundedColorPipelineOutput *radiance_output,
                                    DescriptorSetAllocator *desc_set_allocator,
                                    VulkanContext *context);
    ~DirectionalRadiancePipelineImpl();

    std::unique_ptr<PostProcessorPipeline> sun_light_pipeline;
};

DirectionalRadiancePipeline::DirectionalRadiancePipelineImpl::DirectionalRadiancePipelineImpl(
    UnboundedColorPipelineOutput *radiance_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context) {
    sun_light_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathRadianceDirectional,
        /*input_image_count=*/LightInputsPipelineOutput::ColorOutputCount,
        /*push_constant_size=*/sizeof(SunLightParameters), radiance_output, desc_set_allocator,
        context);
}

DirectionalRadiancePipeline::DirectionalRadiancePipelineImpl::~DirectionalRadiancePipelineImpl() {}

DirectionalRadiancePipeline::DirectionalRadiancePipeline(
    UnboundedColorPipelineOutput *radiance_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : pimpl_(std::make_unique<DirectionalRadiancePipelineImpl>(radiance_output, desc_set_allocator,
                                                               context)) {}

DirectionalRadiancePipeline::~DirectionalRadiancePipeline() {}

UnboundedColorPipelineOutput *
DirectionalRadiancePipeline::Run(SunLight const &light,
                                 LightInputsPipelineOutput const &light_inputs,
                                 GpuPromise const &promise) {
    SunLightPostProcessorConfigurator configurator(light, light_inputs);
    PipelineOutputInterface *output = pimpl_->sun_light_pipeline->Run(configurator, promise);

    return static_cast<UnboundedColorPipelineOutput *>(output);
}

} // namespace e8
