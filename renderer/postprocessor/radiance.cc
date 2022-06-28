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

#include <cmath>
#include <memory>
#include <vector>

#include "common/device.h"
#include "common/tensor.h"
#include "content/proto/light_source.pb.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/postprocessor/radiance.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"
#include "resource/common.h"

namespace e8 {
namespace {

PipelineKey const kSunLightPipeline = "Sun Light";
PipelineKey const kPointLightPipeline = "Point Light";
PipelineKey const kSpotLightPipeline = "Spot Light";

// Base class.
class RadiancePostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    RadiancePostProcessorConfigurator(PipelineOutputInterface const &light_inputs);
    ~RadiancePostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    PipelineOutputInterface const &light_inputs_;
};

RadiancePostProcessorConfigurator::RadiancePostProcessorConfigurator(
    PipelineOutputInterface const &light_inputs)
    : light_inputs_(light_inputs) {}

RadiancePostProcessorConfigurator::~RadiancePostProcessorConfigurator() {}

void RadiancePostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(LightInputsColorOutput::LICO_NORMAL_ROUGHNESS) =
        light_inputs_.ColorAttachments()[LightInputsColorOutput::LICO_NORMAL_ROUGHNESS]->view;

    input_images->at(LightInputsColorOutput::LICO_ALBEDO_METALLIC) =
        light_inputs_.ColorAttachments()[LightInputsColorOutput::LICO_ALBEDO_METALLIC]->view;

    input_images->at(LightInputsColorOutput::LightInputsColorOutputCount) =
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
                                      PipelineOutputInterface const &light_inputs);
    ~SunLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SunLight const &sun_light_;
};

SunLightPostProcessorConfigurator::SunLightPostProcessorConfigurator(
    SunLight const &sun_light, PipelineOutputInterface const &light_inputs)
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

    float rec_x_a;
    float rec_y_a;
    float rec_z_a;
    float rec_z_b;
};

class PointLightPostProcessorConfigurator : public RadiancePostProcessorConfigurator {
  public:
    PointLightPostProcessorConfigurator(PointLight const &point_light,
                                        PipelineOutputInterface const &light_inputs,
                                        frustum const &light_inputs_frustum);
    ~PointLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    PointLight const &point_light_;
    frustum const &light_inputs_frustum_;
};

PointLightPostProcessorConfigurator::PointLightPostProcessorConfigurator(
    PointLight const &point_light, PipelineOutputInterface const &light_inputs,
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

    parameters->rec_x_a = light_inputs_frustum_.XUnprojectionConstant();
    parameters->rec_y_a = light_inputs_frustum_.YUnprojectionConstant();
    parameters->rec_z_a = light_inputs_frustum_.ZUnProjectionConstantA();
    parameters->rec_z_b = light_inputs_frustum_.ZUnprojectionConstantB();
}

// Spot light.
struct SpotLightParameters {
    vec4 position;
    vec4 direction;
    vec4 intensity;
    float cos_outer_cone;
    float cone_normalizer;

    float rec_x_a;
    float rec_y_a;
    float rec_z_a;
    float rec_z_b;
};

class SpotLightPostProcessorConfigurator : public RadiancePostProcessorConfigurator {
  public:
    SpotLightPostProcessorConfigurator(SpotLight const &spot_light,
                                       PipelineOutputInterface const &light_inputs,
                                       frustum const &light_inputs_frustum);
    ~SpotLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SpotLight const &spot_light_;
    frustum const &light_inputs_frustum_;
};

SpotLightPostProcessorConfigurator::SpotLightPostProcessorConfigurator(
    SpotLight const &spot_light, PipelineOutputInterface const &light_inputs,
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
    parameters->cos_outer_cone = std::cos(deg2rad(spot_light_.outer_cone_angle() / 2.0f));

    float cos_inner_cone = std::cos(deg2rad(spot_light_.inner_cone_angle() / 2.0f));
    parameters->cone_normalizer = 1.0f / (cos_inner_cone - parameters->cos_outer_cone);

    parameters->rec_x_a = light_inputs_frustum_.XUnprojectionConstant();
    parameters->rec_y_a = light_inputs_frustum_.YUnprojectionConstant();
    parameters->rec_z_a = light_inputs_frustum_.ZUnProjectionConstantA();
    parameters->rec_z_b = light_inputs_frustum_.ZUnprojectionConstantB();
}

} // namespace

void DoComputeRadiance(LightSourceInstance const &instance, frustum const &view_projection,
                       PipelineStage *light_inputs, PipelineStage *shadow_map,
                       PipelineStage *cleared_radiance_map,
                       DescriptorSetAllocator *desc_set_allocator, VulkanContext *context,
                       PipelineStage *target) {
    CachedPipelineInterface *pipeline;
    std::unique_ptr<PostProcessorConfiguratorInterface> configurator;

    switch (instance.light_source.model_case()) {
    case LightSource::ModelCase::kSunLight: {
        pipeline = target->WithPipeline(
            kSunLightPipeline,
            [desc_set_allocator, context](PipelineOutputInterface *radiance_output) {
                return std::make_unique<PostProcessorPipeline>(
                    kSunLightPipeline, kFragmentShaderFilePathRadianceSunLight,
                    /*input_image_count=*/LightInputsColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(SunLightParameters), radiance_output,
                    desc_set_allocator, context);
            });

        configurator = std::make_unique<SunLightPostProcessorConfigurator>(
            instance.light_source.sun_light(), *light_inputs->Output());
        break;
    }
    case LightSource::ModelCase::kPointLight: {
        pipeline = target->WithPipeline(
            kPointLightPipeline,
            [desc_set_allocator, context](PipelineOutputInterface *radiance_output) {
                return std::make_unique<PostProcessorPipeline>(
                    kPointLightPipeline, kFragmentShaderFilePathRadiancePointLight,
                    /*input_image_count=*/LightInputsColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(PointLightParameters), radiance_output,
                    desc_set_allocator, context);
            });

        configurator = std::make_unique<PointLightPostProcessorConfigurator>(
            instance.light_source.point_light(), *light_inputs->Output(), view_projection);
        break;
    }
    case LightSource::ModelCase::kSpotLight: {
        pipeline = target->WithPipeline(
            kSpotLightPipeline,
            [desc_set_allocator, context](PipelineOutputInterface *radiance_output) {
                return std::make_unique<PostProcessorPipeline>(
                    kSpotLightPipeline, kFragmentShaderFilePathRadianceSpotLight,
                    /*input_image_count=*/LightInputsColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(SpotLightParameters), radiance_output,
                    desc_set_allocator, context);
            });

        configurator = std::make_unique<SpotLightPostProcessorConfigurator>(
            instance.light_source.spot_light(), *light_inputs->Output(), view_projection);
        break;
    }
    default: {
        assert(false);
    }
    }

    target->Schedule(
        pipeline, std::move(configurator),
        /*parents=*/std::vector<PipelineStage *>{light_inputs, shadow_map, cleared_radiance_map});
}

} // namespace e8
