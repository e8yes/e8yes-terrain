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
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/projection.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/drawable/collection.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kProjectSurfacePipeline = "Project Surface Parameters";

unsigned const kMaterialPackageSlot = 2;
unsigned const kPackageSlotCount = 3;

/**
 * @brief The ProjectSurfaceOutput class For storing a 32-bit RGBA color output containing
 * the geometry data as well as a 32-bit depth output.
 */
class ProjectSurfaceOutput final : public GraphicsPipelineOutputInterface {
  public:
    /**
     * @brief ProjectSurfaceOutput Constructs surface parameter projection output with the
     * specified dimension.
     *
     * @param width The width of the surface projection output.
     * @param height The height of the surface projection output.
     * @param context Contextual Vulkan handles.
     */
    ProjectSurfaceOutput(unsigned width, unsigned height, VulkanContext *context)
        : GraphicsPipelineOutputInterface(width, height) {
        normal_roughness_ =
            CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R16G16B16A16_SNORM,
                                  /*transfer_src=*/false, context);
        albedo_metallic_ = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
                                                 /*transfer_src=*/false, context);
        depth_attachment_ = CreateDepthAttachment(width, height, /*samplable=*/true, context);
        render_pass_ = CreateRenderPass(
            /*color_attachments=*/std::vector<VkAttachmentDescription>{normal_roughness_->desc,
                                                                       albedo_metallic_->desc},
            depth_attachment_->desc, context);
        frame_buffer_ = CreateFrameBuffer(
            *render_pass_, width, height,
            /*color_attachments=*/
            std::vector<VkImageView>{normal_roughness_->view, albedo_metallic_->view},
            depth_attachment_->view, context);
    }

    ~ProjectSurfaceOutput() = default;

    FrameBuffer *GetFrameBuffer() const override { return frame_buffer_.get(); }

    RenderPass const &GetRenderPass() const override { return *render_pass_; }

    std::vector<FrameBufferAttachment const *> ColorAttachments() const override {
        return std::vector<FrameBufferAttachment const *>{normal_roughness_.get(),
                                                          albedo_metallic_.get()};
    }

    FrameBufferAttachment const *DepthAttachment() const override {
        return depth_attachment_.get();
    }

  private:
    std::unique_ptr<FrameBufferAttachment> normal_roughness_;
    std::unique_ptr<FrameBufferAttachment> albedo_metallic_;
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

/**
 * @brief CreateProjectSurfaceStage Creates an empty DagOperation with two 32-bit parameter maps and
 * a 32-bit depth map output in the specified dimension. See the SurfaceProjectionColorOutput enum
 * for what information each parameter map contains.
 *
 * @param width The width of the light parameter map output.
 * @param height The width of the light parameter map output.
 * @param context Contextual Vulkan handles.
 * @return The light input stage.
 */
std::unique_ptr<DagOperation> CreateProjectSurfaceOp(unsigned width, unsigned height,
                                                     TransferContext *transfer_context,
                                                     VulkanContext *vulkan_context) {
    auto output = std::make_shared<ProjectSurfaceOutput>(width, height, vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
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

std::vector<ShaderUniformPackageBindings> UniformSlotsLayout() {
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

    std::vector<ShaderUniformPackageBindings> result(kPackageSlotCount);
    result[kMaterialPackageSlot] = ShaderUniformPackageBindings{
        albedo_map_binding, normal_map_binding, roughness_map_binding, metallic_map_binding};
    return result;
}

class MaterialUniforms final : public MaterialUniformsInterface {
  public:
    MaterialUniforms(ImageSampler const *texture_sampler)
        : MaterialUniformsInterface(/*package_slot_index=*/kMaterialPackageSlot,
                                    /*reuse_upload=*/true),
          texture_sampler_(texture_sampler) {}

    UniformPackage UniformsOf(Material const *material) const override {
        UniformPackage result;

        result.texture_packs.push_back(StagingUniformImagePack(
            /*binding=*/0, /*images=*/std::vector<StagingTextureBuffer const *>{&material->albedo},
            texture_sampler_));
        result.texture_packs.push_back(StagingUniformImagePack(
            /*binding=*/1, /*images=*/std::vector<StagingTextureBuffer const *>{&material->normal},
            texture_sampler_));
        result.texture_packs.push_back(StagingUniformImagePack(
            /*binding=*/2,
            /*images=*/std::vector<StagingTextureBuffer const *>{&material->roughness},
            texture_sampler_));
        result.texture_packs.push_back(StagingUniformImagePack(
            /*binding=*/3,
            /*images=*/std::vector<StagingTextureBuffer const *>{&material->metallic},
            texture_sampler_));

        return result;
    }

    ImageSampler const *texture_sampler_;
};

class DrawableUniforms final : public DrawableUniformsInterface {
  public:
    DrawableUniforms(PerspectiveProjection const &projection)
        : DrawableUniformsInterface(/*package_slot_index=*/std::nullopt, /*reuse_upload=*/true),
          projection_(projection) {}

    std::vector<uint8_t> UniformPushConstantsOf(DrawableInstance const &drawable) const override {
        std::vector<uint8_t> bytes(sizeof(PushConstants));

        PushConstants *push_constants = reinterpret_cast<PushConstants *>(bytes.data());
        push_constants->view_model_trans = projection_.ViewTransform() * (*drawable.transform);
        push_constants->proj_view_model_trans =
            projection_.ProjectiveTransform() * push_constants->view_model_trans;

        return bytes;
    }

  private:
    PerspectiveProjection const projection_;
};

/**
 * @brief The ProjectSurfaceArgument struct Arguments to the ProjectSurfacePipeline.
 */
struct ProjectSurfaceArgument : public GraphicsPipelineArgumentsInterface {
    ProjectSurfaceArgument(std::vector<DrawableInstance> const &drawables,
                           PerspectiveProjection const &projection)
        : drawables(drawables), projection(projection) {}

    std::vector<DrawableInstance> drawables;
    PerspectiveProjection projection;
    TransferContext *transfer_context;
};

/**
 * @brief The ProjectSurfacePipeline class For mapping the lighting parameters onto screen.
 */
class ProjectSurfacePipeline final : public GraphicsPipelineInterface {
  public:
    ProjectSurfacePipeline(GraphicsPipelineOutputInterface *output, VulkanContext *context)
        : GraphicsPipelineInterface(context) {
        uniform_layout_ =
            CreateShaderUniformLayout(PushConstantLayout(), UniformSlotsLayout(), context);
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
    }

    ~ProjectSurfacePipeline() override = default;

    PipelineKey Key() const override { return kProjectSurfacePipeline; }

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *output, TransferContext *transfer_context,
                CommandBuffer *command_buffer) override {
        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), command_buffer);

        ProjectSurfaceArgument const &args =
            static_cast<ProjectSurfaceArgument const &>(generic_args);
        MaterialUniforms material_uniforms(texture_sampler_.get());
        DrawableUniforms drawable_uniforms(args.projection);
        RenderDrawables(args.drawables, *pipeline_, *uniform_layout_,
                        RenderPassUniformsInterface::Empty(), material_uniforms, drawable_uniforms,
                        transfer_context, command_buffer);

        FinishRenderPass(command_buffer);
    }
};

} // namespace

DagOperationInstance DoProjectSurface(DrawableCollection *drawable_collection,
                                      PerspectiveProjection const &projection, unsigned width,
                                      unsigned height, DagContext::Session *session) {
    DagOperationInstance target =
        session->WithOperation(kProjectSurfacePipeline, width, height, CreateProjectSurfaceOp);

    GraphicsPipelineInterface *pipeline =
        target->WithPipeline(kProjectSurfacePipeline, [](GraphicsPipelineOutputInterface *output,
                                                         VulkanContext *vulkan_context) {
            return std::make_unique<ProjectSurfacePipeline>(output, vulkan_context);
        });

    ResourceLoadingOption loading_option;
    loading_option.load_geometry = true;
    loading_option.load_material = true;
    std::vector<DrawableInstance> observable_geometries =
        drawable_collection->ObservableGeometries(projection, loading_option);

    auto args = std::make_unique<ProjectSurfaceArgument>(observable_geometries, projection);
    target->Schedule(pipeline, std::move(args),
                     /*parents=*/std::vector<DagOperationInstance>());

    return target;
}

} // namespace e8
