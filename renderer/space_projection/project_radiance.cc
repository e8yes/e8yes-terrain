/**
 * e8yes islands.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/projection.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_buffer.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/drawable/light_source.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/space_projection/project_radiance.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kProjectRadiancePipeline = "Project Radiance";
// unsigned const kSunlightVolumeCount = 4;
unsigned const kMaxShadowedSpotLights = 16;
unsigned const kMaxSpotLights = 64;
unsigned const kMaxPointLights = 64;

struct UniformPushTransforms {
    mat44 view_model_trans;
    mat44 proj_view_model_trans;
};

struct UniformRadianceMapper {
    float viewport_width;
    float viewport_height;
    int num_sun_lights_shadowed;
    int num_spot_lights_shadowed;
    int num_spot_lights;
    int num_point_lights;
};

struct UniformSunLight {
    vec4 direction;
    vec4 intensity;
};

struct UniformSpotLight {
    vec4 position;
    vec4 direction;
    vec4 intensity;
    float cos_outer_cone;
    float cone_normalizer;
};

struct UniformShadowedSpotLight {
    mat44 to_light_space;
    vec4 position;
    vec4 direction;
    vec4 intensity;
    float cos_outer_cone;
    float cone_normalizer;
};

struct UniformPointLight {
    vec4 position;
    vec4 intensity;
};

std::unique_ptr<DagOperation> CreateProjectRadianceOp(unsigned width, unsigned height,
                                                      TransferContext *transfer_context,
                                                      VulkanContext *vulkan_context) {
    auto output = std::make_shared<HdrColorOutput>(width, height, /*with_depth_buffer=*/false,
                                                   vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
}

VkPushConstantRange PushConstantLayout() {
    VkPushConstantRange push_constant{};
    push_constant.size = sizeof(UniformPushTransforms);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    return push_constant;
}

std::vector<VkVertexInputAttributeDescription> VertexShaderInputAttributes() {
    VkVertexInputAttributeDescription position_attribute{};
    position_attribute.binding = 0;
    position_attribute.location = 0;
    position_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    position_attribute.offset = offsetof(PrimitiveVertex, position);

    VkVertexInputAttributeDescription tex_coord_attribute{};
    tex_coord_attribute.binding = 0;
    tex_coord_attribute.location = 1;
    tex_coord_attribute.format = VK_FORMAT_R32G32_SFLOAT;
    tex_coord_attribute.offset = offsetof(PrimitiveVertex, tex_coord);

    return std::vector<VkVertexInputAttributeDescription>{position_attribute, tex_coord_attribute};
}

std::vector<VkDescriptorSetLayoutBinding> PerPassDescriptorSetBindings() {
    VkDescriptorSetLayoutBinding radiance_mapper_constants{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    VkDescriptorSetLayoutBinding sunlight{
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    //    VkDescriptorSetLayoutBinding sunlight_shadow_maps{
    //        .binding = 2,
    //        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    //        .descriptorCount = kSunlightVolumeCount,
    //        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    //    };

    VkDescriptorSetLayoutBinding spotlight_shadowed{
        .binding = 3,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    //    VkDescriptorSetLayoutBinding spotlight_shadow_maps{
    //        .binding = 4,
    //        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    //        .descriptorCount = kMaxShadowedSpotLights,
    //        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    //    };

    VkDescriptorSetLayoutBinding spotlight{
        .binding = 5,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    VkDescriptorSetLayoutBinding pointlight{
        .binding = 6,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    VkDescriptorSetLayoutBinding screen_normal_map_binding{
        .binding = 7,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    return std::vector<VkDescriptorSetLayoutBinding>{
        radiance_mapper_constants, sunlight, spotlight_shadowed, spotlight, pointlight,
        screen_normal_map_binding};
}

std::vector<VkDescriptorSetLayoutBinding> PerDrawableDescriptorSetBindings() {

    VkDescriptorSetLayoutBinding albedo_map_binding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    VkDescriptorSetLayoutBinding roughness_map_binding{
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    VkDescriptorSetLayoutBinding metallic_map_binding{
        .binding = 2,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
    };

    return std::vector<VkDescriptorSetLayoutBinding>{albedo_map_binding, roughness_map_binding,
                                                     metallic_map_binding};
}

class ProjectRadianceConfigurator final : public RenderPassConfiguratorInterface {
  public:
    ProjectRadianceConfigurator(ProjectionInterface const &projection,
                                ImageSampler const &texture_sampler)
        : projection_(projection), texture_sampler_(texture_sampler) {}

    ~ProjectRadianceConfigurator() = default;

    bool IncludeDrawable(DrawableInstance const &drawable) const override {
        return drawable.material != nullptr;
    }

    std::vector<uint8_t> PushConstantOf(DrawableInstance const &drawable) const override {
        std::vector<uint8_t> bytes(sizeof(UniformPushTransforms));

        UniformPushTransforms *transforms = reinterpret_cast<UniformPushTransforms *>(bytes.data());
        transforms->view_model_trans = projection_.ViewTransform() * (*drawable.transform);
        transforms->proj_view_model_trans =
            projection_.ProjectiveTransform() * transforms->view_model_trans;

        return bytes;
    }

    TextureSelector TexturesOf(DrawableInstance const &drawable) const override {
        TextureSelector selector(drawable.material->id);

        selector.sampler = &texture_sampler_;

        selector.textures[TextureType::TT_ALBEDO] = &drawable.material->albedo;
        selector.bindings[TextureType::TT_ALBEDO] = 0;

        selector.textures[TextureType::TT_ROUGHNESS] = &drawable.material->roughness;
        selector.bindings[TextureType::TT_ROUGHNESS] = 1;

        selector.textures[TextureType::TT_METALLIC] = &drawable.material->metallic;
        selector.bindings[TextureType::TT_METALLIC] = 2;

        return selector;
    }

  private:
    ProjectionInterface const &projection_;
    ImageSampler const &texture_sampler_;
};

struct ProjectRadianceArgument : public GraphicsPipelineArgumentsInterface {
    ProjectRadianceArgument(std::vector<DrawableInstance> const &drawables,
                            std::optional<ShadowedSunLight> const shadowed_sun_light,
                            std::vector<ShadowedSpotLight> const shadowed_spot_lights,
                            std::vector<LightSourceInstance> const spot_lights,
                            std::vector<LightSourceInstance> const point_lights,
                            ProjectionInterface const &projection)
        : drawables(drawables), shadowed_sun_light(shadowed_sun_light),
          shadowed_spot_lights(shadowed_spot_lights), spot_lights(spot_lights),
          point_lights(point_lights), projection(projection) {}

    std::vector<DrawableInstance> const drawables;
    std::optional<ShadowedSunLight> const shadowed_sun_light;
    std::vector<ShadowedSpotLight> const shadowed_spot_lights;
    std::vector<LightSourceInstance> const spot_lights;
    std::vector<LightSourceInstance> const point_lights;
    ProjectionInterface const &projection;
};

class ProjectRadiancePipeline final : public GraphicsPipelineInterface {
  public:
    ProjectRadiancePipeline(GraphicsPipelineOutputInterface *output, VulkanContext *context)
        : GraphicsPipelineInterface(context) {
        uniform_layout_ = CreateShaderUniformLayout(
            PushConstantLayout(),
            /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
            /*per_pass_desc_set=*/PerPassDescriptorSetBindings(),
            /*per_drawable_desc_set=*/PerDrawableDescriptorSetBindings(), context);
        shader_stages_ = CreateShaderStages(
            /*vertex_shader_file_path=*/kVertexShaderFilePathLightInputs,
            /*fragment_shader_file_path=*/kFragmentShaderFilePathLightInputs, context);
        vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
        fixed_stage_config_ =
            CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                   /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                   /*enable_depth_test=*/true, output->Width(), output->Height(),
                                   /*color_attachment_count=*/2);

        pipeline_ =
            CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                   *vertex_inputs_, *fixed_stage_config_, context);

        texture_sampler_ = CreateTextureSampler(context);

        radiance_mapper_ubo_ = CreateUniformBuffer(sizeof(UniformRadianceMapper), context);
        sun_light_ubo_ = CreateUniformBuffer(sizeof(UniformSunLight), context);
        shadowed_spot_light_ubo_ =
            CreateUniformBuffer(sizeof(UniformShadowedSpotLight) * kMaxShadowedSpotLights, context);
        spot_light_ubo_ = CreateUniformBuffer(sizeof(UniformSpotLight) * kMaxSpotLights, context);
        point_light_ubo_ =
            CreateUniformBuffer(sizeof(UniformPointLight) * kMaxPointLights, context);
    }

    ~ProjectRadiancePipeline() override = default;

    PipelineKey Key() const override { return kProjectRadiancePipeline; }

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *output, TransferContext *transfer_context,
                CommandBuffer *command_buffer) override {
        ProjectRadianceArgument const &args =
            static_cast<ProjectRadianceArgument const &>(generic_args);

        // TODO: Configure uniforms here.
        if (pass_uniform_desc_set_ == nullptr) {
            pass_uniform_desc_set_ = transfer_context->descriptor_set_allocator.Allocate(
                DescriptorType::DT_UNIFORM_BUFFER, uniform_layout_->per_pass_desc_set);
        }
        if (pass_image_desc_set_ == nullptr) {
            pass_image_desc_set_ = transfer_context->descriptor_set_allocator.Allocate(
                DescriptorType::DT_COMBINED_IMAGE_SAMPLER, uniform_layout_->per_pass_desc_set);
        }

        UniformRadianceMapper uniform_radiance_mapper;
        uniform_radiance_mapper.viewport_width = output->Width();
        uniform_radiance_mapper.viewport_height = output->Height();
        uniform_radiance_mapper.num_sun_lights_shadowed = args.shadowed_sun_light.has_value();
        uniform_radiance_mapper.num_spot_lights_shadowed = args.shadowed_spot_lights.size();
        uniform_radiance_mapper.num_spot_lights = args.spot_lights.size();
        uniform_radiance_mapper.num_point_lights = args.point_lights.size();

        UniformSunLight uniform_sun_light;
        if (args.shadowed_sun_light.has_value()) {
            args.shadowed_sun_light->sun_light.light_source.sun_light();
        }

        std::vector<UniformShadowedSpotLight> shadowed_spot_lights;
        shadowed_spot_lights.reserve(args.shadowed_spot_lights.size());

        std::vector<UniformSpotLight> spot_lights;
        spot_lights.reserve(args.spot_lights.size());

        spot_lights.data();

        std::vector<VkDescriptorSet> descriptor_sets{};
        vkCmdBindDescriptorSets(
            command_buffer->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout_->layout,
            /*firstSet=*/DescriptorSetType::DST_PER_PASS,
            /*descriptorSetCount=*/descriptor_sets.size(), descriptor_sets.data(),
            /*dynamicOffsetCount=*/0,
            /*pDynamicOffsets=*/nullptr);

        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), command_buffer);
        ProjectRadianceConfigurator configurator(args.projection, *texture_sampler_);
        RenderDrawables(args.drawables, *pipeline_, *uniform_layout_, configurator,
                        transfer_context, command_buffer);
        FinishRenderPass(command_buffer);
    }

  private:
    std::unique_ptr<UniformBuffer> radiance_mapper_ubo_;
    std::unique_ptr<UniformBuffer> sun_light_ubo_;
    std::unique_ptr<UniformBuffer> shadowed_spot_light_ubo_;
    std::unique_ptr<UniformBuffer> spot_light_ubo_;
    std::unique_ptr<UniformBuffer> point_light_ubo_;
    std::unique_ptr<DescriptorSet> pass_uniform_desc_set_;
    std::unique_ptr<DescriptorSet> pass_image_desc_set_;
};

} // namespace

DagOperationInstance DoProjectRadiance(std::vector<DrawableInstance> const &drawables,
                                       std::optional<ShadowedSunLight> const &shadowed_sun_light,
                                       std::vector<ShadowedSpotLight> const &shadowed_spot_lights,
                                       std::vector<LightSourceInstance> const &spot_lights,
                                       std::vector<LightSourceInstance> const &point_lights,
                                       PerspectiveProjection const &projection,
                                       DagOperationInstance projected_surface,
                                       DagContext::Session *session) {
    DagOperationInstance target =
        session->WithOperation(kProjectRadiancePipeline, projected_surface->Output()->Width(),
                               projected_surface->Output()->Height(), CreateProjectRadianceOp);

    GraphicsPipelineInterface *pipeline =
        target->WithPipeline(kProjectRadiancePipeline, [](GraphicsPipelineOutputInterface *output,
                                                          TransferContext * /*transfer_context*/,
                                                          VulkanContext *vulkan_context) {
            return std::make_unique<ProjectRadiancePipeline>(output, vulkan_context);
        });

    auto args = std::make_unique<ProjectRadianceArgument>(
        drawables, shadowed_sun_light, shadowed_spot_lights, spot_lights, point_lights, projection);
    target->Schedule(pipeline, std::move(args),
                     /*parents=*/std::vector<DagOperationInstance>());

    return target;
}

} // namespace e8
