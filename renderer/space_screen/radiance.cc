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

#include "common/tensor.h"
#include "content/proto/light_source.pb.h"
#include "renderer/basic/shader.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/drawable/light_source.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/space_screen/radiance.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "resource/common.h"

namespace e8 {
namespace {

PipelineKey const kRadiancePipeline = "Radiance";
PipelineKey const kSunLightPipeline = "Sun Light";
PipelineKey const kPointLightPipeline = "Point Light";
PipelineKey const kSpotLightPipeline = "Spot Light";

// Base class.
class RadiancePostProcessorConfigurator : public ScreenSpaceConfiguratorInterface {
  public:
    RadiancePostProcessorConfigurator(GraphicsPipelineOutputInterface const &projected_surface);
    ~RadiancePostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    GraphicsPipelineOutputInterface const &projected_surface_;
};

RadiancePostProcessorConfigurator::RadiancePostProcessorConfigurator(
    GraphicsPipelineOutputInterface const &projected_surface)
    : projected_surface_(projected_surface) {}

RadiancePostProcessorConfigurator::~RadiancePostProcessorConfigurator() {}

void RadiancePostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(SurfaceProjectionColorOutput::LICO_NORMAL_ROUGHNESS) =
        projected_surface_.ColorAttachments()[SurfaceProjectionColorOutput::LICO_NORMAL_ROUGHNESS]
            ->view;

    input_images->at(SurfaceProjectionColorOutput::LICO_ALBEDO_METALLIC) =
        projected_surface_.ColorAttachments()[SurfaceProjectionColorOutput::LICO_ALBEDO_METALLIC]
            ->view;

    input_images->at(SurfaceProjectionColorOutput::LightInputsColorOutputCount) =
        projected_surface_.DepthAttachment()->view;
}

// Sun light.
struct SunLightParameters {
    vec4 direction;
    vec4 intensity;
};

class SunLightPostProcessorConfigurator : public RadiancePostProcessorConfigurator {
  public:
    SunLightPostProcessorConfigurator(SunLight const &sun_light,
                                      GraphicsPipelineOutputInterface const &projected_surface);
    ~SunLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SunLight sun_light_;
};

SunLightPostProcessorConfigurator::SunLightPostProcessorConfigurator(
    SunLight const &sun_light, GraphicsPipelineOutputInterface const &projected_surface)
    : RadiancePostProcessorConfigurator(projected_surface), sun_light_(sun_light) {}

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
                                        GraphicsPipelineOutputInterface const &projected_surface,
                                        frustum const &light_inputs_frustum);
    ~PointLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    PointLight point_light_;
    frustum light_inputs_frustum_;
};

PointLightPostProcessorConfigurator::PointLightPostProcessorConfigurator(
    PointLight const &point_light, GraphicsPipelineOutputInterface const &projected_surface,
    frustum const &light_inputs_frustum)
    : RadiancePostProcessorConfigurator(projected_surface), point_light_(point_light),
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
                                       GraphicsPipelineOutputInterface const &light_inputs,
                                       frustum const &light_inputs_frustum);
    ~SpotLightPostProcessorConfigurator() override;

    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    SpotLight spot_light_;
    frustum light_inputs_frustum_;
};

SpotLightPostProcessorConfigurator::SpotLightPostProcessorConfigurator(
    SpotLight const &spot_light, GraphicsPipelineOutputInterface const &light_inputs,
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

std::unique_ptr<DagOperation> CreateRadianceOp(unsigned width, unsigned height,
                                               TransferContext *transfer_context,
                                               VulkanContext *vulkan_context) {
    auto output = std::make_shared<HdrColorOutput>(width, height, /*with_depth_buffer=*/false,
                                                   vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
}

} // namespace

DagOperationInstance DoComputeRadiance(LightSourceInstance const &instance,
                                       DagOperationInstance projected_surface,
                                       frustum const &projection,
                                       std::vector<DagOperationInstance> const &shadow_maps,
                                       DagContext::Session *session) {
    DagOperationInstance target =
        session->WithOperation(kRadiancePipeline, projected_surface->Output()->Width(),
                               projected_surface->Output()->Height(), CreateRadianceOp);

    GraphicsPipelineInterface *pipeline;
    std::unique_ptr<ScreenSpaceConfiguratorInterface> configurator;

    switch (instance.light_source.model_case()) {
    case LightSource::ModelCase::kSunLight: {
        pipeline = target->WithPipeline(
            kSunLightPipeline,
            [](GraphicsPipelineOutputInterface *radiance_output, TransferContext *transfer_context,
               VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kSunLightPipeline, kFragmentShaderFilePathRadianceSunLight,
                    /*input_image_count=*/
                    SurfaceProjectionColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(SunLightParameters), radiance_output,
                    transfer_context, vulkan_context);
            });

        configurator = std::make_unique<SunLightPostProcessorConfigurator>(
            instance.light_source.sun_light(), *projected_surface->Output());
        break;
    }
    case LightSource::ModelCase::kPointLight: {
        pipeline = target->WithPipeline(
            kPointLightPipeline,
            [](GraphicsPipelineOutputInterface *radiance_output, TransferContext *transfer_context,
               VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kPointLightPipeline, kFragmentShaderFilePathRadiancePointLight,
                    /*input_image_count=*/
                    SurfaceProjectionColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(PointLightParameters), radiance_output,
                    transfer_context, vulkan_context);
            });

        configurator = std::make_unique<PointLightPostProcessorConfigurator>(
            instance.light_source.point_light(), *projected_surface->Output(), projection);
        break;
    }
    case LightSource::ModelCase::kSpotLight: {
        pipeline = target->WithPipeline(
            kSpotLightPipeline,
            [](GraphicsPipelineOutputInterface *radiance_output, TransferContext *transfer_context,
               VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kSpotLightPipeline, kFragmentShaderFilePathRadianceSpotLight,
                    /*input_image_count=*/
                    SurfaceProjectionColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(SpotLightParameters), radiance_output,
                    transfer_context, vulkan_context);
            });

        configurator = std::make_unique<SpotLightPostProcessorConfigurator>(
            instance.light_source.spot_light(), *projected_surface->Output(), projection);
        break;
    }
    default: {
        assert(false);
    }
    }

    std::vector<DagOperationInstance> depending_parents{projected_surface};
    for (auto shadow_map : shadow_maps) {
        depending_parents.push_back(shadow_map);
    }

    target->Schedule(pipeline, std::move(configurator), depending_parents);
    return target;
}

} // namespace e8
