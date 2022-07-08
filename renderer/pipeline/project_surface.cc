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

#include <vulkan/vulkan.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

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
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/pass/configurator.h"
#include "renderer/pass/rasterize.h"
#include "renderer/pipeline/project_surface.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/texture_group.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"

namespace e8 {
namespace {

PipelineKey const kProjectSurfacePipeline = "Project Surface Parameters";

/**
 * @brief The ProjectSurfacePipelineOutput class For storing a 32-bit RGBA color output containing
 * the geometry data as well as a 32-bit depth output.
 */
class ProjectSurfacePipelineOutput : public PipelineOutputInterface {
   public:
    /**
     * @brief ProjectSurfacePipelineOutput Constructs surface parameter projection output with the
     * specified dimension.
     *
     * @param width The width of the surface projection output.
     * @param height The height of the surface projection output.
     * @param context Contextual Vulkan handles.
     */
    ProjectSurfacePipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~ProjectSurfacePipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

   private:
    struct LightInputsPipelineOutputImpl;
    std::unique_ptr<LightInputsPipelineOutputImpl> pimpl_;
};

struct ProjectSurfacePipelineOutput::LightInputsPipelineOutputImpl {
    LightInputsPipelineOutputImpl(unsigned width, unsigned height, VulkanContext *context);
    ~LightInputsPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> normal_roughness_;
    std::unique_ptr<FrameBufferAttachment> albedo_metallic_;
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;

    VulkanContext *context;
};

ProjectSurfacePipelineOutput::LightInputsPipelineOutputImpl::LightInputsPipelineOutputImpl(
    unsigned width, unsigned height, VulkanContext *context) {
    normal_roughness_ = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R16G16B16A16_SNORM,
                                              /*transfer_src=*/false, context);
    albedo_metallic_ = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
                                             /*transfer_src=*/false, context);
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

ProjectSurfacePipelineOutput::LightInputsPipelineOutputImpl::~LightInputsPipelineOutputImpl() {}

ProjectSurfacePipelineOutput::ProjectSurfacePipelineOutput(unsigned width, unsigned height,
                                                           VulkanContext *context)
    : PipelineOutputInterface(width, height),
      pimpl_(std::make_unique<LightInputsPipelineOutputImpl>(width, height, context)) {}

ProjectSurfacePipelineOutput::~ProjectSurfacePipelineOutput() {}

FrameBuffer *ProjectSurfacePipelineOutput::GetFrameBuffer() const {
    return pimpl_->frame_buffer_.get();
}

RenderPass const &ProjectSurfacePipelineOutput::GetRenderPass() const {
    return *pimpl_->render_pass_;
}

std::vector<FrameBufferAttachment const *> ProjectSurfacePipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{pimpl_->normal_roughness_.get(),
                                                      pimpl_->albedo_metallic_.get()};
}

FrameBufferAttachment const *ProjectSurfacePipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
}

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

/**
 * @brief The RenderPassConfigurator class For setting up the light inputs shader's uniform
 * variables.
 */
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

std::vector<uint8_t> RenderPassConfigurator::PushConstantOf(
    DrawableInstance const &drawable) const {
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

/**
 * @brief The LightInputsPipelineArgument struct Arguments to the LightInputsPipeline.
 */
struct LightInputsPipelineArgument : public CachedPipelineArgumentsInterface {
    LightInputsPipelineArgument(std::vector<DrawableInstance> const &drawables,
                                ProjectionInterface const &projection,
                                TextureDescriptorSetCache *tex_desc_set_cache,
                                GeometryVramTransfer *geo_vram, TextureVramTransfer *tex_vram)
        : drawables(drawables),
          projection(projection),
          tex_desc_set_cache(tex_desc_set_cache),
          geo_vram(geo_vram),
          tex_vram(tex_vram) {}

    std::vector<DrawableInstance> const &drawables;
    ProjectionInterface const &projection;
    TextureDescriptorSetCache *tex_desc_set_cache;
    GeometryVramTransfer *geo_vram;
    TextureVramTransfer *tex_vram;
};

/**
 * @brief The ProjectSurfacePipeline class For mapping the lighting parameters onto screen.
 */
class ProjectSurfacePipeline : public CachedPipelineInterface {
   public:
    ProjectSurfacePipeline(PipelineOutputInterface *output, VulkanContext *context);
    ~ProjectSurfacePipeline() override;

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;
};

ProjectSurfacePipeline::ProjectSurfacePipeline(PipelineOutputInterface *output,
                                               VulkanContext *context)
    : CachedPipelineInterface(context) {
    uniform_layout_ = CreateShaderUniformLayout(
        PushConstantLayout(), /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_drawable_desc_set=*/DescriptorSetBindings(), context);
    shader_stages_ = CreateShaderStages(
        /*vertex_shader_file_path=*/kVertexShaderFilePathLightInputs,
        /*fragment_shader_file_path=*/kFragmentShaderFilePathLightInputs, context);
    vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
    fixed_stage_config_ = CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                                 /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                                 /*enable_depth_test=*/true, output->Width(),
                                                 output->Height(), /*color_attachment_count=*/2);

    pipeline_ = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                       *vertex_inputs_, *fixed_stage_config_, context);

    texture_sampler_ = CreateTextureSampler(context);
}

ProjectSurfacePipeline::~ProjectSurfacePipeline() {}

PipelineKey ProjectSurfacePipeline::Key() const { return kProjectSurfacePipeline; }

Fulfillment ProjectSurfacePipeline::Launch(CachedPipelineArgumentsInterface const &generic_args,
                                           std::vector<GpuPromise *> const &prerequisites,
                                           unsigned completion_signal_count,
                                           PipelineOutputInterface *output) {
    VkCommandBuffer cmds =
        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), context_);

    LightInputsPipelineArgument const &args =
        static_cast<LightInputsPipelineArgument const &>(generic_args);

    RenderPassConfigurator configurator(args.projection, *texture_sampler_);
    RenderDrawables(args.drawables, *pipeline_, *uniform_layout_, configurator,
                    args.tex_desc_set_cache, args.geo_vram, args.tex_vram, cmds);

    return FinishRenderPass(cmds, completion_signal_count, prerequisites, context_);
}

}  // namespace

std::unique_ptr<PipelineStage> CreateProjectSurfaceStage(unsigned width, unsigned height,
                                                         VulkanContext *context) {
    auto output = std::make_shared<ProjectSurfacePipelineOutput>(width, height, context);
    return std::make_unique<PipelineStage>(output);
}

void DoProjectSurface(std::vector<DrawableInstance> const &drawables,
                      ProjectionInterface const &projection,
                      TextureDescriptorSetCache *tex_desc_set_cache, GeometryVramTransfer *geo_vram,
                      TextureVramTransfer *tex_vram, VulkanContext *context,
                      PipelineStage *first_stage, PipelineStage *target) {
    CachedPipelineInterface *pipeline =
        target->WithPipeline(kProjectSurfacePipeline, [context](PipelineOutputInterface *output) {
            return std::make_unique<ProjectSurfacePipeline>(output, context);
        });

    auto args = std::make_unique<LightInputsPipelineArgument>(
        drawables, projection, tex_desc_set_cache, geo_vram, tex_vram);
    target->Schedule(pipeline, std::move(args),
                     /*parents=*/std::vector<PipelineStage *>{first_stage});
}

}  // namespace e8
