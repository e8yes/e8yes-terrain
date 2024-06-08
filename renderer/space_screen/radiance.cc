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
class RadianceUniforms : public ScreenSpaceUniformsInterface {
  public:
    RadianceUniforms(GraphicsPipelineOutputInterface const *projected_surface)
        : projected_surface_(projected_surface) {}
    ~RadianceUniforms() override = default;

    void InputImages(std::vector<VkImageView> *input_images) const override {
        input_images->at(SurfaceProjectionColorOutput::LICO_NORMAL_ROUGHNESS) =
            projected_surface_
                ->ColorAttachments()[SurfaceProjectionColorOutput::LICO_NORMAL_ROUGHNESS]
                ->view;

        input_images->at(SurfaceProjectionColorOutput::LICO_ALBEDO_METALLIC) =
            projected_surface_
                ->ColorAttachments()[SurfaceProjectionColorOutput::LICO_ALBEDO_METALLIC]
                ->view;

        input_images->at(SurfaceProjectionColorOutput::LightInputsColorOutputCount) =
            projected_surface_->DepthAttachment()->view;
    }

  private:
    GraphicsPipelineOutputInterface const *projected_surface_;
};

// Sun light.
struct SunLightParameters {
    vec4 direction;
    vec4 intensity;
};

class SunLightUniforms : public RadianceUniforms {
  public:
    SunLightUniforms(SunLight const &sun_light,
                     GraphicsPipelineOutputInterface const *projected_surface)
        : RadianceUniforms(projected_surface), sun_light_(sun_light) {}

    ~SunLightUniforms() override = default;

    void PushConstants(std::vector<uint8_t> *push_constants) const override {
        SunLightParameters *parameters =
            reinterpret_cast<SunLightParameters *>(push_constants->data());

        parameters->direction = ToVec3(sun_light_.direction()).homo(0.0f);
        parameters->intensity = ToVec3(sun_light_.intensity()).homo(1.0f);
    }

  private:
    SunLight sun_light_;
};

// Point light.
struct PointLightParameters {
    vec4 position;
    vec4 intensity;

    float rec_x_a;
    float rec_y_a;
    float rec_z_a;
    float rec_z_b;
};

class PointLightUniforms : public RadianceUniforms {
  public:
    PointLightUniforms(PointLight const &point_light, frustum const &surface_projection,
                       GraphicsPipelineOutputInterface const *projected_surface)
        : RadianceUniforms(projected_surface), point_light_(point_light),
          surface_projection_(surface_projection) {}

    ~PointLightUniforms() override = default;

    void PushConstants(std::vector<uint8_t> *push_constants) const override {
        PointLightParameters *parameters =
            reinterpret_cast<PointLightParameters *>(push_constants->data());

        parameters->position = ToVec3(point_light_.position()).homo(1.0f);
        parameters->intensity = ToVec3(point_light_.intensity()).homo(1.0f);

        parameters->rec_x_a = surface_projection_.XUnprojectionConstant();
        parameters->rec_y_a = surface_projection_.YUnprojectionConstant();
        parameters->rec_z_a = surface_projection_.ZUnProjectionConstantA();
        parameters->rec_z_b = surface_projection_.ZUnprojectionConstantB();
    }

  private:
    PointLight point_light_;
    frustum surface_projection_;
};

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

class SpotLightUniforms : public RadianceUniforms {
  public:
    SpotLightUniforms(SpotLight const &spot_light, frustum const &surface_projection,
                      GraphicsPipelineOutputInterface const *projected_surface)
        : RadianceUniforms(projected_surface), spot_light_(spot_light),
          surface_projection_(surface_projection) {}

    ~SpotLightUniforms() override = default;

    void PushConstants(std::vector<uint8_t> *push_constants) const override {
        SpotLightParameters *parameters =
            reinterpret_cast<SpotLightParameters *>(push_constants->data());

        parameters->position = ToVec3(spot_light_.position()).homo(1.0f);
        parameters->direction = ToVec3(spot_light_.direction()).homo(0.0f);
        parameters->intensity = ToVec3(spot_light_.intensity()).homo(1.0f);
        parameters->cos_outer_cone = std::cos(deg2rad(spot_light_.outer_cone_angle() / 2.0f));

        float cos_inner_cone = std::cos(deg2rad(spot_light_.inner_cone_angle() / 2.0f));
        parameters->cone_normalizer = 1.0f / (cos_inner_cone - parameters->cos_outer_cone);

        parameters->rec_x_a = surface_projection_.XUnprojectionConstant();
        parameters->rec_y_a = surface_projection_.YUnprojectionConstant();
        parameters->rec_z_a = surface_projection_.ZUnProjectionConstantA();
        parameters->rec_z_b = surface_projection_.ZUnprojectionConstantB();
    }

  private:
    SpotLight spot_light_;
    frustum surface_projection_;
};

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
    std::unique_ptr<ScreenSpaceUniformsInterface> uniforms;

    switch (instance.light_source.model_case()) {
    case LightSource::ModelCase::kSunLight: {
        pipeline = target->WithPipeline(
            kSunLightPipeline,
            [](GraphicsPipelineOutputInterface *radiance_output, VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kSunLightPipeline, kFragmentShaderFilePathRadianceSunLight,
                    /*input_image_count=*/
                    SurfaceProjectionColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(SunLightParameters), radiance_output,
                    vulkan_context);
            });

        uniforms = std::make_unique<SunLightUniforms>(instance.light_source.sun_light(),
                                                      projected_surface->Output());
        break;
    }
    case LightSource::ModelCase::kPointLight: {
        pipeline = target->WithPipeline(
            kPointLightPipeline,
            [](GraphicsPipelineOutputInterface *radiance_output, VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kPointLightPipeline, kFragmentShaderFilePathRadiancePointLight,
                    /*input_image_count=*/
                    SurfaceProjectionColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(PointLightParameters), radiance_output,
                    vulkan_context);
            });

        uniforms = std::make_unique<PointLightUniforms>(instance.light_source.point_light(),
                                                        projection, projected_surface->Output());
        break;
    }
    case LightSource::ModelCase::kSpotLight: {
        pipeline = target->WithPipeline(
            kSpotLightPipeline,
            [](GraphicsPipelineOutputInterface *radiance_output, VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kSpotLightPipeline, kFragmentShaderFilePathRadianceSpotLight,
                    /*input_image_count=*/
                    SurfaceProjectionColorOutput::LightInputsColorOutputCount + 1,
                    /*push_constant_size=*/sizeof(SpotLightParameters), radiance_output,
                    vulkan_context);
            });

        uniforms = std::make_unique<SpotLightUniforms>(instance.light_source.spot_light(),
                                                       projection, projected_surface->Output());
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

    target->Schedule(pipeline, std::move(uniforms), depending_parents);
    return target;
}

} // namespace e8
