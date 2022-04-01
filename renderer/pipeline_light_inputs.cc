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

#include <cassert>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_light_inputs.h"
#include "renderer/pipeline_output.h"
#include "renderer/projection.h"
#include "renderer/render_pass.h"
#include "renderer/vram_geometry.h"
#include "renderer/vram_texture.h"

namespace e8 {
namespace {

struct PushConstants {
    mat44 view_model_trans;
    mat44 proj_view_model_trans;
};

VkPushConstantRange PushConstantLayout() {
    VkPushConstantRange push_constant{};
    push_constant.size = sizeof(PushConstants);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    return push_constant;
}

std::vector<VkVertexInputAttributeDescription> VertexShaderInputAttributes() {
    VkVertexInputAttributeDescription position_attribute{};
    position_attribute.binding = 0;
    position_attribute.location = 0;
    position_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    position_attribute.offset = offsetof(PrimitiveVertex, position);

    VkVertexInputAttributeDescription normal_attribute{};
    normal_attribute.binding = 0;
    normal_attribute.location = 1;
    normal_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    normal_attribute.offset = offsetof(PrimitiveVertex, normal);

    VkVertexInputAttributeDescription tangent_attribute{};
    tangent_attribute.binding = 0;
    tangent_attribute.location = 2;
    tangent_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    tangent_attribute.offset = offsetof(PrimitiveVertex, tangent);

    VkVertexInputAttributeDescription tex_coord_attribute{};
    tex_coord_attribute.binding = 0;
    tex_coord_attribute.location = 3;
    tex_coord_attribute.format = VK_FORMAT_R32G32_SFLOAT;
    tex_coord_attribute.offset = offsetof(PrimitiveVertex, tex_coord);

    return std::vector<VkVertexInputAttributeDescription>{position_attribute, normal_attribute,
                                                          tangent_attribute, tex_coord_attribute};
}

std::vector<VkDescriptorSetLayoutBinding> NormalRoughnessMapBinding() {
    VkDescriptorSetLayoutBinding normal_map_binding{};
    normal_map_binding.binding = 0;
    normal_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    normal_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normal_map_binding.descriptorCount = 1;

    VkDescriptorSetLayoutBinding roughness_map_binding{};
    normal_map_binding.binding = 1;
    normal_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    normal_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normal_map_binding.descriptorCount = 1;

    return std::vector<VkDescriptorSetLayoutBinding>{normal_map_binding, roughness_map_binding};
}

class RenderPassConfigurator : public RenderPassConfiguratorInterface {
  public:
    RenderPassConfigurator(ProjectionInterface const &projection,
                           ShaderUniformLayout const &uniform_layout,
                           DescriptorSets const &descriptor_sets,
                           ImageSampler const &normal_map_sampler,
                           ImageSampler const &roughness_map_sampler, VulkanContext *context);
    ~RenderPassConfigurator();

    bool IncludeDrawable(DrawableInstance const &drawable) const override;
    DrawableTextures TexturesOf(DrawableInstance const &drawable) const override;
    void SetUniformsFor(DrawableInstance const &drawable, DrawableGpuTextures const &textures,
                        VkCommandBuffer cmds) const override;

  private:
    ProjectionInterface const &projection_;
    ShaderUniformLayout const &uniform_layout_;
    DescriptorSets const &descriptor_sets_;
    ImageSampler const &normal_map_sampler_;
    ImageSampler const &roughness_map_sampler_;
    VulkanContext *context_;
};

RenderPassConfigurator::RenderPassConfigurator(ProjectionInterface const &projection,
                                               ShaderUniformLayout const &uniform_layout,
                                               DescriptorSets const &descriptor_sets,
                                               ImageSampler const &normal_map_sampler,
                                               ImageSampler const &roughness_map_sampler,
                                               VulkanContext *context)
    : projection_(projection), uniform_layout_(uniform_layout), descriptor_sets_(descriptor_sets),
      normal_map_sampler_(normal_map_sampler), roughness_map_sampler_(roughness_map_sampler),
      context_(context) {}

RenderPassConfigurator::~RenderPassConfigurator() {}

bool RenderPassConfigurator::IncludeDrawable(DrawableInstance const &drawable) const {
    return drawable.material;
}

RenderPassConfigurator::DrawableTextures
RenderPassConfigurator::TexturesOf(DrawableInstance const &drawable) const {
    RenderPassConfigurator::DrawableTextures textures;

    textures.staging_textures[NORMAL] = &drawable.material->normal;
    textures.staging_textures[ROUGHNESS] = &drawable.material->roughness;

    return textures;
}

void RenderPassConfigurator::SetUniformsFor(DrawableInstance const &drawable,
                                            DrawableGpuTextures const &textures,
                                            VkCommandBuffer cmds) const {
    // Vertex shader push constants.
    PushConstants push_constants;
    push_constants.view_model_trans = projection_.ViewTransform() * (*drawable.transform);
    push_constants.proj_view_model_trans =
        projection_.ProjectiveTransform() * push_constants.view_model_trans;

    vkCmdPushConstants(cmds, uniform_layout_.layout,
                       /*stageFlags=*/VK_SHADER_STAGE_VERTEX_BIT,
                       /*offset=*/0,
                       /*size=*/sizeof(PushConstants), &push_constants);

    // Fragment shader texture map descriptors.
    WriteImageDescriptor(textures.gpu_textures[NORMAL]->view, normal_map_sampler_,
                         descriptor_sets_.drawable,
                         /*binding=*/0, context_);
    WriteImageDescriptor(textures.gpu_textures[ROUGHNESS]->view, roughness_map_sampler_,
                         descriptor_sets_.drawable,
                         /*binding=*/1, context_);

    vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout_.layout,
                            /*firstSet=*/0,
                            /*descriptorSetCount=*/1, &descriptor_sets_.drawable,
                            /*dynamicOffsetCount=*/0,
                            /*pDynamicOffsets=*/nullptr);
}

} // namespace

class LightInputsPipeline::LightInputsPipelineImpl {
  public:
    LightInputsPipelineImpl(PipelineOutputInterface *output, VulkanContext *context);
    ~LightInputsPipelineImpl();

  public:
    PipelineOutputInterface *output;
    VulkanContext *context;

  public:
    std::unique_ptr<ShaderStages> shader_stages;
    std::unique_ptr<ShaderUniformLayout> uniform_layout;
    std::unique_ptr<DescriptorSets> descriptor_sets;
    std::unique_ptr<VertexInputInfo> vertex_inputs;
    std::unique_ptr<FixedStageConfig> fixed_stage_config;

    std::unique_ptr<GraphicsPipeline> pipeline;

    std::unique_ptr<ImageSampler> normal_map_sampler;
    std::unique_ptr<ImageSampler> roughness_map_sampler;
};

LightInputsPipeline::LightInputsPipelineImpl::LightInputsPipelineImpl(
    PipelineOutputInterface *output, VulkanContext *context)
    : output(output), context(context) {
    uniform_layout = CreateShaderUniformLayout(
        PushConstantLayout(), /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_drawable_desc_set=*/NormalRoughnessMapBinding(), context);
    descriptor_sets = CreateDescriptorSets(*uniform_layout, context);
    shader_stages = CreateShaderStages(
        /*vertex_shader_file_path=*/kVertexShaderFilePathLightInputs,
        /*fragment_shader_file_path=*/kFragmentShaderFilePathLightInputs, context);
    vertex_inputs = CreateVertexInputState(VertexShaderInputAttributes());
    fixed_stage_config =
        CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                               /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                               /*enable_depth_test=*/true, output->width, output->height);

    pipeline = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages, *uniform_layout,
                                      *vertex_inputs, *fixed_stage_config, context);

    normal_map_sampler = CreateTextureSampler(context);
    roughness_map_sampler = CreateTextureSampler(context);
}

LightInputsPipeline::LightInputsPipelineImpl::~LightInputsPipelineImpl() {}

LightInputsPipeline::LightInputsPipeline(PipelineOutputInterface *output, VulkanContext *context)
    : pimpl_(std::make_unique<LightInputsPipelineImpl>(output, context)) {}

LightInputsPipeline::~LightInputsPipeline() {}

PipelineOutputInterface *LightInputsPipeline::Run(std::vector<DrawableInstance> const &drawables,
                                                  ProjectionInterface const &projection,
                                                  GpuBarrier const &prerequisites,
                                                  GeometryVramTransfer *geo_vram,
                                                  TextureVramTransfer *tex_vram) {
    VkCommandBuffer cmds = StartRenderPass(pimpl_->output->GetRenderPass(),
                                           *pimpl_->output->GetFrameBuffer(), pimpl_->context);

    RenderPassConfigurator configurator(projection, *pimpl_->uniform_layout,
                                        *pimpl_->descriptor_sets, *pimpl_->normal_map_sampler,
                                        *pimpl_->roughness_map_sampler, pimpl_->context);
    RenderDrawables(drawables, *pimpl_->pipeline, configurator, geo_vram, tex_vram, cmds);

    pimpl_->output->barrier =
        FinishRenderPass(cmds, prerequisites, pimpl_->output->AcquireFence(), pimpl_->context);

    return pimpl_->output;
}

} // namespace e8
