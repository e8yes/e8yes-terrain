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
#include "renderer/postprocessor/radiance.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"
#include "resource/common.h"

namespace e8 {
namespace {

// Base class.
class RadiancePostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    RadiancePostProcessorConfigurator(LightInputsPipelineOutput const &light_inputs);
    ~RadiancePostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    LightInputsPipelineOutput const &light_inputs_;
};

RadiancePostProcessorConfigurator::RadiancePostProcessorConfigurator(
    LightInputsPipelineOutput const &light_inputs)
    : light_inputs_(light_inputs) {}

RadiancePostProcessorConfigurator::~RadiancePostProcessorConfigurator() {}

void RadiancePostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(LightInputsPipelineOutput::NORMAL_ROUGHNESS) =
        light_inputs_.ColorAttachments()[LightInputsPipelineOutput::NORMAL_ROUGHNESS]->view;

    input_images->at(LightInputsPipelineOutput::ALBEDO_METALLIC) =
        light_inputs_.ColorAttachments()[LightInputsPipelineOutput::ALBEDO_METALLIC]->view;

    input_images->at(LightInputsPipelineOutput::ColorOutputCount) =
        light_inputs_.DepthAttachment()->view;
}

// Sun light.
struct SunLightParameters {
    vec4 direction;
    vec4 intensity;
};

class SunLightPostProcessorConfigurator : public RadiancePostProcessorConfigurator {
  public:
    SunLightPostProcessorConfigurator(SunLight const &sun_light,
                                      LightInputsPipelineOutput const &light_inputs);
    ~SunLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SunLight const &sun_light_;
};

SunLightPostProcessorConfigurator::SunLightPostProcessorConfigurator(
    SunLight const &sun_light, LightInputsPipelineOutput const &light_inputs)
    : RadiancePostProcessorConfigurator(light_inputs), sun_light_(sun_light) {}

SunLightPostProcessorConfigurator::~SunLightPostProcessorConfigurator() {}

void SunLightPostProcessorConfigurator::PushConstants(std::vector<uint8_t> *push_constants) const {
    SunLightParameters *parameters = reinterpret_cast<SunLightParameters *>(push_constants->data());

    parameters->direction = ToVec3(sun_light_.direction()).homo(0.0f);
    parameters->intensity = ToVec3(sun_light_.intensity()).homo(1.0f);
}

// Point light.
struct PointLightParameters {
    vec4 position;
    vec4 intensity;

    float sensor_width;
    float sensor_height;
    float z_near;
    float z_far;
};

class PointLightPostProcessorConfigurator : public RadiancePostProcessorConfigurator {
  public:
    PointLightPostProcessorConfigurator(PointLight const &point_light,
                                        LightInputsPipelineOutput const &light_inputs,
                                        frustum const &light_inputs_frustum);
    ~PointLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    PointLight const &point_light_;
    frustum const &light_inputs_frustum_;
};

PointLightPostProcessorConfigurator::PointLightPostProcessorConfigurator(
    PointLight const &point_light, LightInputsPipelineOutput const &light_inputs,
    frustum const &light_inputs_frustum)
    : RadiancePostProcessorConfigurator(light_inputs), point_light_(point_light),
      light_inputs_frustum_(light_inputs_frustum) {}

PointLightPostProcessorConfigurator::~PointLightPostProcessorConfigurator() {}

void PointLightPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> *push_constants) const {
    PointLightParameters *parameters =
        reinterpret_cast<PointLightParameters *>(push_constants->data());

    parameters->position = ToVec3(point_light_.position()).homo(1.0f);
    parameters->intensity = ToVec3(point_light_.intensity()).homo(1.0f);

    parameters->sensor_width = light_inputs_frustum_.right - light_inputs_frustum_.left;
    parameters->sensor_height = light_inputs_frustum_.top - light_inputs_frustum_.bottom;
    parameters->z_near = light_inputs_frustum_.z_near;
    parameters->z_far = light_inputs_frustum_.z_far;
}

// Spot light.
struct SpotLightParameters {
    vec4 position;
    vec4 direction;
    vec4 intensity;
    float inner_cone_angle;
    float outer_cone_angle;

    float sensor_width;
    float sensor_height;
    float z_near;
    float z_far;
};

class SpotLightPostProcessorConfigurator : public RadiancePostProcessorConfigurator {
  public:
    SpotLightPostProcessorConfigurator(SpotLight const &spot_light,
                                       LightInputsPipelineOutput const &light_inputs,
                                       frustum const &light_inputs_frustum);
    ~SpotLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SpotLight const &spot_light_;
    frustum const &light_inputs_frustum_;
};

SpotLightPostProcessorConfigurator::SpotLightPostProcessorConfigurator(
    SpotLight const &spot_light, LightInputsPipelineOutput const &light_inputs,
    frustum const &light_inputs_frustum)
    : RadiancePostProcessorConfigurator(light_inputs), spot_light_(spot_light),
      light_inputs_frustum_(light_inputs_frustum) {}

SpotLightPostProcessorConfigurator::~SpotLightPostProcessorConfigurator() {}

void SpotLightPostProcessorConfigurator::PushConstants(std::vector<uint8_t> *push_constants) const {
    SpotLightParameters *parameters =
        reinterpret_cast<SpotLightParameters *>(push_constants->data());

    parameters->position = ToVec3(spot_light_.position()).homo(1.0f);
    parameters->direction = ToVec3(spot_light_.direction()).homo(0.0f);
    parameters->intensity = ToVec3(spot_light_.intensity()).homo(1.0f);
    parameters->inner_cone_angle = spot_light_.inner_cone_angle();
    parameters->outer_cone_angle = spot_light_.outer_cone_angle();

    parameters->sensor_width = light_inputs_frustum_.right - light_inputs_frustum_.left;
    parameters->sensor_height = light_inputs_frustum_.top - light_inputs_frustum_.bottom;
    parameters->z_near = light_inputs_frustum_.z_near;
    parameters->z_far = light_inputs_frustum_.z_far;
}

} // namespace

struct RadiancePipeline::DirectionalRadiancePipelineImpl {
    DirectionalRadiancePipelineImpl(UnboundedColorPipelineOutput *radiance_output,
                                    DescriptorSetAllocator *desc_set_allocator,
                                    VulkanContext *context);
    ~DirectionalRadiancePipelineImpl();

    std::unique_ptr<PostProcessorPipeline> sun_light_pipeline;
    std::unique_ptr<PostProcessorPipeline> point_light_pipeline;
    std::unique_ptr<PostProcessorPipeline> spot_light_pipeline;
};

RadiancePipeline::DirectionalRadiancePipelineImpl::DirectionalRadiancePipelineImpl(
    UnboundedColorPipelineOutput *radiance_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context) {
    sun_light_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathRadianceSunLight,
        /*input_image_count=*/LightInputsPipelineOutput::ColorOutputCount + 1,
        /*push_constant_size=*/sizeof(SunLightParameters), radiance_output, desc_set_allocator,
        context);

    point_light_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathRadiancePointLight,
        /*input_image_count=*/LightInputsPipelineOutput::ColorOutputCount + 1,
        /*push_constant_size=*/sizeof(PointLightParameters), radiance_output, desc_set_allocator,
        context);

    //    spot_light_pipeline = std::make_unique<PostProcessorPipeline>(
    //        kFragmentShaderFilePathRadianceSpotLight,
    //        /*input_image_count=*/LightInputsPipelineOutput::ColorOutputCount + 1,
    //        /*push_constant_size=*/sizeof(SpotLightParameters), radiance_output,
    //        desc_set_allocator, context);
}

RadiancePipeline::DirectionalRadiancePipelineImpl::~DirectionalRadiancePipelineImpl() {}

RadiancePipeline::RadiancePipeline(UnboundedColorPipelineOutput *radiance_output,
                                   DescriptorSetAllocator *desc_set_allocator,
                                   VulkanContext *context)
    : pimpl_(std::make_unique<DirectionalRadiancePipelineImpl>(radiance_output, desc_set_allocator,
                                                               context)) {}

RadiancePipeline::~RadiancePipeline() {}

UnboundedColorPipelineOutput *RadiancePipeline::Run(LightSourceInstance const &instance,
                                                    LightInputsPipelineOutput const &light_inputs,
                                                    frustum const &light_inputs_frustum,
                                                    GpuPromise const &promise) {
    PipelineOutputInterface *output;

    switch (instance.light_source.model_case()) {
    case LightSource::ModelCase::kSunLight: {
        SunLightPostProcessorConfigurator configurator(instance.light_source.sun_light(),
                                                       light_inputs);
        output = pimpl_->sun_light_pipeline->Run(configurator, promise);
        break;
    }
    case LightSource::ModelCase::kPointLight: {
        PointLightPostProcessorConfigurator configurator(instance.light_source.point_light(),
                                                         light_inputs, light_inputs_frustum);
        output = pimpl_->point_light_pipeline->Run(configurator, promise);
        break;
    }
    case LightSource::ModelCase::kSpotLight: {
        SpotLightPostProcessorConfigurator configurator(instance.light_source.spot_light(),
                                                        light_inputs, light_inputs_frustum);
        output = pimpl_->spot_light_pipeline->Run(configurator, promise);
        break;
    }
    default: {
        assert(false);
    }
    }

    return static_cast<UnboundedColorPipelineOutput *>(output);
}

} // namespace e8
