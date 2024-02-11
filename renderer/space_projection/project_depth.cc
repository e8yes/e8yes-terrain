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
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/projection.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/drawable/collection.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/space_projection/project_depth.h"

namespace e8 {
namespace {

PipelineKey const kProjectDepthPipeline = "Project Depth";
PipelineKey const kProjectLinearDepthPipeline = "Project Linear Depth";

/**
 * @brief The ProjectDepthOutput class For storing, depth-only rendering output. The depth
 * values are stored in 32-bit floats.
 */
class ProjectDepthOutput final : public GraphicsPipelineOutputInterface {
  public:
    /**
     * @brief ProjectDepthOutput Constructs a depth map output with the specified dimension.
     *
     * @param width The width of the depth map output.
     * @param height The height of the depth map output.
     * @param context Contextual Vulkan handles.
     */
    ProjectDepthOutput(unsigned width, unsigned height, VulkanContext *context)
        : GraphicsPipelineOutputInterface(width, height) {
        depth_attachment_ = CreateDepthAttachment(width, height, /*samplable=*/true, context);
        render_pass_ =
            CreateRenderPass(/*color_attachments=*/std::vector<VkAttachmentDescription>(),
                             depth_attachment_->desc, context);
        frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                          /*color_attachments=*/std::vector<VkImageView>(),
                                          depth_attachment_->view, context);
    }

    ~ProjectDepthOutput() = default;

    FrameBuffer *GetFrameBuffer() const override { return frame_buffer_.get(); }

    RenderPass const &GetRenderPass() const override { return *render_pass_; }

    std::vector<FrameBufferAttachment const *> ColorAttachments() const override {
        return std::vector<FrameBufferAttachment const *>{};
    }

    FrameBufferAttachment const *DepthAttachment() const override {
        return depth_attachment_.get();
    }

  private:
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

/**
 * @brief CreateProjectNdcDepthOp Creates a depth projection pipeline stage with a 32-bit depth-only
 * output in the specified dimension.
 *
 * @param width The width of the depth map output.
 * @param height The height of the depth map output.
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the depth map output.
 */
std::unique_ptr<DagOperation> CreateProjectNdcDepthOp(unsigned width, unsigned height,
                                                      VulkanContext *context) {
    auto output = std::make_shared<ProjectDepthOutput>(width, height, context);
    return std::make_unique<DagOperation>(output);
}

/**
 * @brief CreateProjectLinearDepthOp Creates a linear depth projection pipeline stage with a 32-bit
 * floating point color output in the specified dimension. TODO: preserves the depth attachment.
 *
 * @param width The width of the depth map output.
 * @param height The height of the depth map output.
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the depth map output.
 */
std::unique_ptr<DagOperation> CreateProjectLinearDepthOp(unsigned width, unsigned height,
                                                         VulkanContext *context) {
    auto output = std::make_shared<FloatOutput>(width, height, /*with_depth_buffer=*/true, context);
    return std::make_unique<DagOperation>(output);
}

struct PushConstant {
    mat44 model_view_proj_trans;
};

VkPushConstantRange PushConstantLayout() {
    VkPushConstantRange push_constant{};
    push_constant.size = sizeof(PushConstant);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    return push_constant;
}

std::vector<VkVertexInputAttributeDescription> VertexShaderInputAttributes() {
    VkVertexInputAttributeDescription position_attribute{};
    position_attribute.binding = 0;
    position_attribute.location = 0;
    position_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    position_attribute.offset = offsetof(PrimitiveVertex, position);

    return std::vector<VkVertexInputAttributeDescription>{position_attribute};
}

class ProjectDepthConfigurator final : public RenderPassConfiguratorInterface {
  public:
    ProjectDepthConfigurator(ProjectionInterface const &projection) : projection_(projection) {}

    ~ProjectDepthConfigurator() = default;

    std::vector<uint8_t> PushConstantOf(DrawableInstance const &drawable) const override {
        std::vector<uint8_t> bytes(sizeof(PushConstant));

        PushConstant *push_constant = reinterpret_cast<PushConstant *>(bytes.data());
        push_constant->model_view_proj_trans =
            projection_.ProjectiveTransform() * projection_.ViewTransform() * (*drawable.transform);

        return bytes;
    }

  private:
    ProjectionInterface const &projection_;
};

struct ProjectDepthArguments final : public GraphicsPipelineArgumentsInterface {
    ProjectDepthArguments(std::vector<DrawableInstance> const &drawables,
                          PerspectiveProjection const &projection,
                          TransferContext *transfer_context)
        : drawables(drawables), projection(new PerspectiveProjection(projection)),
          transfer_context(transfer_context) {}

    ~ProjectDepthArguments() { delete projection; }

    std::vector<DrawableInstance> drawables;
    ProjectionInterface const *projection;
    TransferContext *transfer_context;
};

class ProjectDepthPipeline final : public GraphicsPipelineInterface {
  public:
    ProjectDepthPipeline(ProjectDepthOutput *output, VulkanContext *context)
        : GraphicsPipelineInterface(context) {
        uniform_layout_ = CreateShaderUniformLayout(
            PushConstantLayout(),
            /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
            /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
            /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
        shader_stages_ =
            CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathDepthMap,
                               /*fragment_shader_file_path=*/std::nullopt, context);
        vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
        fixed_stage_config_ =
            CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                   /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                   /*enable_depth_test=*/true, output->Width(), output->Height(),
                                   /*color_attachment_count=*/0);

        pipeline_ =
            CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                   *vertex_inputs_, *fixed_stage_config_, context);
    }

    ~ProjectDepthPipeline() override = default;

    PipelineKey Key() const override { return kProjectDepthPipeline; }

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *output, CommandBuffer *command_buffer) override {
        ProjectDepthArguments const &args =
            static_cast<ProjectDepthArguments const &>(generic_args);

        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), command_buffer);
        ProjectDepthConfigurator configurator(*args.projection);
        RenderDrawables(args.drawables, *pipeline_, *uniform_layout_, configurator,
                        args.transfer_context, command_buffer);
        FinishRenderPass(command_buffer);
    }
};

class ProjectLinearDepthPipeline final : public GraphicsPipelineInterface {
  public:
    ProjectLinearDepthPipeline(GraphicsPipelineOutputInterface *output, VulkanContext *context)
        : GraphicsPipelineInterface(context) {
        uniform_layout_ = CreateShaderUniformLayout(
            PushConstantLayout(),
            /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
            /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
            /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
        shader_stages_ = CreateShaderStages(
            /*vertex_shader_file_path=*/kVertexShaderFilePathDepthMap,
            /*fragment_shader_file_path=*/kFragmentShaderFilePathDepthMapLinear, context);
        vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
        fixed_stage_config_ =
            CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                   /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                   /*enable_depth_test=*/true, output->Width(), output->Height(),
                                   /*color_attachment_count=*/1);

        pipeline_ =
            CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                   *vertex_inputs_, *fixed_stage_config_, context);
    }

    ~ProjectLinearDepthPipeline() override = default;

    PipelineKey Key() const override { return kProjectLinearDepthPipeline; }

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *output, CommandBuffer *command_buffer) override {
        ProjectDepthArguments const &args =
            static_cast<ProjectDepthArguments const &>(generic_args);

        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), command_buffer);
        ProjectDepthConfigurator configurator(*args.projection);
        RenderDrawables(args.drawables, *pipeline_, *uniform_layout_, configurator,
                        args.transfer_context, command_buffer);
        FinishRenderPass(command_buffer);
    }
};

} // namespace

DagOperationInstance DoProjectNdcDepth(DrawableCollection *drawable_collection,
                                       PerspectiveProjection const &projection,
                                       DagOperationInstance const frame, TransferContext *transfer,
                                       DagContext *dag) {
    unsigned width = frame->Output()->Width();
    unsigned height = frame->Output()->Height();
    DagContext::DagOperationKey key = CreateDagOperationKey(kProjectDepthPipeline, width, height);
    DagOperationInstance projected_ndc_depth =
        dag->WithOperation(key, [width, height](VulkanContext *context) {
            return CreateProjectNdcDepthOp(width, height, context);
        });
    GraphicsPipelineInterface *project_depth_pipeline = projected_ndc_depth->WithPipeline(
        kProjectDepthPipeline, [transfer](GraphicsPipelineOutputInterface *output) {
            return std::make_unique<ProjectDepthPipeline>(
                dynamic_cast<ProjectDepthOutput *>(output), transfer->vulkan_context);
        });

    ResourceLoadingOption loading_option;
    loading_option.load_geometry = true;
    std::vector<DrawableInstance> observable_geometries =
        drawable_collection->ObservableGeometries(projection, loading_option);

    auto args =
        std::make_unique<ProjectDepthArguments>(observable_geometries, projection, transfer);
    projected_ndc_depth->Schedule(project_depth_pipeline, std::move(args),
                                  /*parents=*/std::vector<DagOperation *>{frame});

    return projected_ndc_depth;
}

DagOperationInstance DoProjectLinearDepth(DrawableCollection *drawable_collection,
                                          PerspectiveProjection const &projection, unsigned width,
                                          unsigned height, DagOperationInstance const dependent_op,
                                          TransferContext *transfer, DagContext *dag) {
    DagContext::DagOperationKey key = CreateDagOperationKey(kProjectDepthPipeline, width, height);
    DagOperationInstance projected_linear_depth =
        dag->WithOperation(key, [width, height](VulkanContext *context) {
            return CreateProjectLinearDepthOp(width, height, context);
        });
    GraphicsPipelineInterface *project_linear_depth_pipeline = projected_linear_depth->WithPipeline(
        kProjectLinearDepthPipeline, [transfer](GraphicsPipelineOutputInterface *output) {
            return std::make_unique<ProjectLinearDepthPipeline>(output, transfer->vulkan_context);
        });

    ResourceLoadingOption loading_option;
    loading_option.load_geometry = true;
    std::vector<DrawableInstance> observable_geometries =
        drawable_collection->ObservableGeometries(projection, loading_option);

    auto args =
        std::make_unique<ProjectDepthArguments>(observable_geometries, projection, transfer);
    projected_linear_depth->Schedule(project_linear_depth_pipeline, std::move(args),
                                     /*parents=*/std::vector<DagOperation *>{dependent_op});

    return projected_linear_depth;
}

} // namespace e8
