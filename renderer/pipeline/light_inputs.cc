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

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/projection.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pass/configurator.h"
#include "renderer/pass/rasterize.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/texture_group.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"

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

    VkVertexInputAttributeDescription bitangent_sign_attribute{};
    bitangent_sign_attribute.binding = 0;
    bitangent_sign_attribute.location = 3;
    bitangent_sign_attribute.format = VK_FORMAT_R32_SFLOAT;
    bitangent_sign_attribute.offset = offsetof(PrimitiveVertex, bitangent_sign);

    VkVertexInputAttributeDescription tex_coord_attribute{};
    tex_coord_attribute.binding = 0;
    tex_coord_attribute.location = 4;
    tex_coord_attribute.format = VK_FORMAT_R32G32_SFLOAT;
    tex_coord_attribute.offset = offsetof(PrimitiveVertex, tex_coord);

    return std::vector<VkVertexInputAttributeDescription>{
        position_attribute, normal_attribute, tangent_attribute, bitangent_sign_attribute,
        tex_coord_attribute};
}

std::vector<VkDescriptorSetLayoutBinding> DescriptorSetBindings() {
    VkDescriptorSetLayoutBinding albedo_map_binding{};
    albedo_map_binding.binding = 0;
    albedo_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    albedo_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    albedo_map_binding.descriptorCount = 1;

    VkDescriptorSetLayoutBinding normal_map_binding{};
    normal_map_binding.binding = 1;
    normal_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    normal_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normal_map_binding.descriptorCount = 1;

    VkDescriptorSetLayoutBinding roughness_map_binding{};
    roughness_map_binding.binding = 2;
    roughness_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    roughness_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    roughness_map_binding.descriptorCount = 1;

    VkDescriptorSetLayoutBinding metallic_map_binding{};
    metallic_map_binding.binding = 3;
    metallic_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    metallic_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    metallic_map_binding.descriptorCount = 1;

    return std::vector<VkDescriptorSetLayoutBinding>{albedo_map_binding, normal_map_binding,
                                                     roughness_map_binding, metallic_map_binding};
}

class RenderPassConfigurator : public RenderPassConfiguratorInterface {
  public:
    RenderPassConfigurator(ProjectionInterface const &projection,
                           ImageSampler const &texture_sampler);
    ~RenderPassConfigurator();

    bool IncludeDrawable(DrawableInstance const &drawable) const override;
    std::vector<uint8_t> PushConstantOf(DrawableInstance const &drawable) const override;
    TextureSelector TexturesOf(DrawableInstance const &drawable) const override;

  private:
    ProjectionInterface const &projection_;
    ImageSampler const &texture_sampler_;
};

RenderPassConfigurator::RenderPassConfigurator(ProjectionInterface const &projection,
                                               ImageSampler const &texture_sampler)
    : projection_(projection), texture_sampler_(texture_sampler) {}

RenderPassConfigurator::~RenderPassConfigurator() {}

bool RenderPassConfigurator::IncludeDrawable(DrawableInstance const &drawable) const {
    return drawable.material != nullptr;
}

std::vector<uint8_t>
RenderPassConfigurator::PushConstantOf(DrawableInstance const &drawable) const {
    std::vector<uint8_t> bytes(sizeof(PushConstants));

    PushConstants *push_constants = reinterpret_cast<PushConstants *>(bytes.data());
    push_constants->view_model_trans = projection_.ViewTransform() * (*drawable.transform);
    push_constants->proj_view_model_trans =
        projection_.ProjectiveTransform() * push_constants->view_model_trans;

    return bytes;
}

TextureSelector RenderPassConfigurator::TexturesOf(DrawableInstance const &drawable) const {
    TextureSelector selector(drawable.material->id);

    selector.sampler = &texture_sampler_;

    selector.textures[TextureType::TT_ALBEDO] = &drawable.material->albedo;
    selector.bindings[TextureType::TT_ALBEDO] = 0;

    selector.textures[TextureType::TT_NORMAL] = &drawable.material->normal;
    selector.bindings[TextureType::TT_NORMAL] = 1;

    selector.textures[TextureType::TT_ROUGHNESS] = &drawable.material->roughness;
    selector.bindings[TextureType::TT_ROUGHNESS] = 2;

    selector.textures[TextureType::TT_METALLIC] = &drawable.material->metallic;
    selector.bindings[TextureType::TT_METALLIC] = 3;

    return selector;
}

} // namespace

struct LightInputsPipelineOutput::LightInputsPipelineOutputImpl {
    LightInputsPipelineOutputImpl(unsigned width, unsigned height, VulkanContext *context);
    ~LightInputsPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> normal_roughness_;
    std::unique_ptr<FrameBufferAttachment> albedo_metallic_;
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;

    VulkanContext *context;
};

LightInputsPipelineOutput::LightInputsPipelineOutputImpl::LightInputsPipelineOutputImpl(
    unsigned width, unsigned height, VulkanContext *context) {
    normal_roughness_ =
        CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R16G16B16A16_SNORM, context);
    albedo_metallic_ =
        CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, context);
    depth_attachment_ = CreateDepthAttachment(width, height, /*samplable=*/true, context);
    render_pass_ = CreateRenderPass(
        /*color_attachments=*/std::vector<VkAttachmentDescription>{normal_roughness_->desc,
                                                                   albedo_metallic_->desc},
        depth_attachment_->desc, context);
    frame_buffer_ =
        CreateFrameBuffer(*render_pass_, width, height,
                          /*color_attachments=*/
                          std::vector<VkImageView>{normal_roughness_->view, albedo_metallic_->view},
                          depth_attachment_->view, context);
}

LightInputsPipelineOutput::LightInputsPipelineOutputImpl::~LightInputsPipelineOutputImpl() {}

LightInputsPipelineOutput::LightInputsPipelineOutput(unsigned width, unsigned height,
                                                     VulkanContext *context)
    : PipelineOutputInterface(width, height, context),
      pimpl_(std::make_unique<LightInputsPipelineOutputImpl>(width, height, context)) {}

LightInputsPipelineOutput::~LightInputsPipelineOutput() {}

FrameBuffer *LightInputsPipelineOutput::GetFrameBuffer() const {
    return pimpl_->frame_buffer_.get();
}

RenderPass const &LightInputsPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass_; }

std::vector<FrameBufferAttachment const *> LightInputsPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{pimpl_->normal_roughness_.get(),
                                                      pimpl_->albedo_metallic_.get()};
}

FrameBufferAttachment const *LightInputsPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
}

class LightInputsPipeline::LightInputsPipelineImpl {
  public:
    LightInputsPipelineImpl(LightInputsPipelineOutput *output, VulkanContext *context);
    ~LightInputsPipelineImpl();

  public:
    LightInputsPipelineOutput *output;
    VulkanContext *context;

  public:
    std::unique_ptr<ShaderStages> shader_stages;
    std::unique_ptr<ShaderUniformLayout> uniform_layout;
    std::unique_ptr<VertexInputInfo> vertex_inputs;
    std::unique_ptr<FixedStageConfig> fixed_stage_config;

    std::unique_ptr<GraphicsPipeline> pipeline;

    std::unique_ptr<ImageSampler> texture_sampler;
};

LightInputsPipeline::LightInputsPipelineImpl::LightInputsPipelineImpl(
    LightInputsPipelineOutput *output, VulkanContext *context)
    : output(output), context(context) {
    uniform_layout = CreateShaderUniformLayout(
        PushConstantLayout(), /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_drawable_desc_set=*/DescriptorSetBindings(), context);
    shader_stages = CreateShaderStages(
        /*vertex_shader_file_path=*/kVertexShaderFilePathLightInputs,
        /*fragment_shader_file_path=*/kFragmentShaderFilePathLightInputs, context);
    vertex_inputs = CreateVertexInputState(VertexShaderInputAttributes());
    fixed_stage_config = CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                                /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                                /*enable_depth_test=*/true, output->Width(),
                                                output->Height(), /*color_attachment_count=*/2);

    pipeline = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages, *uniform_layout,
                                      *vertex_inputs, *fixed_stage_config, context);

    texture_sampler = CreateTextureSampler(context);
}

LightInputsPipeline::LightInputsPipelineImpl::~LightInputsPipelineImpl() {}

LightInputsPipeline::LightInputsPipeline(LightInputsPipelineOutput *output, VulkanContext *context)
    : pimpl_(std::make_unique<LightInputsPipelineImpl>(output, context)) {}

LightInputsPipeline::~LightInputsPipeline() {}

LightInputsPipelineOutput *LightInputsPipeline::Run(std::vector<DrawableInstance> const &drawables,
                                                    ProjectionInterface const &projection,
                                                    GpuPromise const &prerequisites,
                                                    TextureDescriptorSetCache *tex_desc_set_cache,
                                                    GeometryVramTransfer *geo_vram,
                                                    TextureVramTransfer *tex_vram) {
    VkCommandBuffer cmds = StartRenderPass(pimpl_->output->GetRenderPass(),
                                           *pimpl_->output->GetFrameBuffer(), pimpl_->context);

    RenderPassConfigurator configurator(projection, *pimpl_->texture_sampler);
    RenderDrawables(drawables, *pimpl_->pipeline, *pimpl_->uniform_layout, configurator,
                    tex_desc_set_cache, geo_vram, tex_vram, cmds);

    RenderPassPromise promise = FinishRenderPass(cmds, prerequisites, pimpl_->context);
    pimpl_->output->AddWriter(std::move(promise.gpu), std::move(promise.cpu));

    return pimpl_->output;
}

} // namespace e8
