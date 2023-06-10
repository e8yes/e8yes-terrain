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
#include "renderer/dag/promise.h"
#include "renderer/drawable/collection.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/space_projection/project_depth.h"
#include "renderer/space_screen/screen_space_processor.h"

namespace e8 {
namespace {

PipelineKey const kProjectDepthPipeline = "Project Depth";
PipelineKey const kLinearizeDepthPipeline = "Linearize Depth";

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
 * @brief The LinearDepthOutput class For storing, floating point texture render output.
 * The linearized depth values are stored in 32-bit floats.
 */
class LinearDepthOutput final : public GraphicsPipelineOutputInterface {
  public:
    LinearDepthOutput(unsigned width, unsigned height, VulkanContext *context)
        : GraphicsPipelineOutputInterface(width, height) {
        fp_attachment_ = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R32_SFLOAT,
                                               /*transfer_src=*/false, context);
        render_pass_ = CreateRenderPass(
            /*color_attachments=*/std::vector<VkAttachmentDescription>{fp_attachment_->desc},
            /*depth_attachment_desc=*/std::nullopt, context);
        frame_buffer_ =
            CreateFrameBuffer(*render_pass_, width, height,
                              /*color_attachments=*/std::vector<VkImageView>{fp_attachment_->view},
                              /*depth_attachment=*/std::nullopt, context);
    }

    ~LinearDepthOutput() = default;

    FrameBuffer *GetFrameBuffer() const override { return frame_buffer_.get(); }

    RenderPass const &GetRenderPass() const override { return *render_pass_; }

    std::vector<FrameBufferAttachment const *> ColorAttachments() const override {
        return std::vector<FrameBufferAttachment const *>{fp_attachment_.get()};
    }

    FrameBufferAttachment const *DepthAttachment() const override { return nullptr; }

  private:
    std::unique_ptr<FrameBufferAttachment> fp_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

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

    Fulfillment Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count,
                       GraphicsPipelineOutputInterface *output) override {
        ProjectDepthArguments const &args =
            static_cast<ProjectDepthArguments const &>(generic_args);

        VkCommandBuffer cmds =
            StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), context_);

        ProjectDepthConfigurator configurator(*args.projection);
        RenderDrawables(args.drawables, *pipeline_, *uniform_layout_, configurator,
                        args.transfer_context, cmds);

        return FinishRenderPass(cmds, completion_signal_count, prerequisites, context_);
    }
};

struct LinearizeDepthPipelineParameters {
    float z_near;
    float z_far;
};

class LinearizeDepthPipelineConfigurator final : public ScreenSpaceConfiguratorInterface {
  public:
    LinearizeDepthPipelineConfigurator(PerspectiveProjection const &projection,
                                       GraphicsPipelineOutputInterface const &depth_map)
        : projection_(projection), depth_map_(depth_map) {}

    ~LinearizeDepthPipelineConfigurator() = default;

    void InputImages(std::vector<VkImageView> *input_images) const override {
        input_images->at(0) = depth_map_.DepthAttachment()->view;
    }

    void PushConstants(std::vector<uint8_t> *push_constants) const override {
        LinearizeDepthPipelineParameters *parameters =
            reinterpret_cast<LinearizeDepthPipelineParameters *>(push_constants->data());

        parameters->z_near = projection_.Frustum().z_near;
        parameters->z_far = projection_.Frustum().z_far;
    }

  private:
    PerspectiveProjection const projection_;
    GraphicsPipelineOutputInterface const &depth_map_;
};

} // namespace

std::unique_ptr<DagOperation> CreateProjectNdcDepthStage(unsigned width, unsigned height,
                                                         VulkanContext *context) {
    auto output = std::make_shared<ProjectDepthOutput>(width, height, context);
    return std::make_unique<DagOperation>(output);
}

std::unique_ptr<DagOperation> CreateProjectLinearDepthStage(unsigned width, unsigned height,
                                                            VulkanContext *context) {
    auto output = std::make_shared<LinearDepthOutput>(width, height, context);
    return std::make_unique<DagOperation>(output);
}

void DoProjectDepth(DrawableCollection *drawables, PerspectiveProjection const &projection,
                    TransferContext *transfer_context, DagOperation *first_stage,
                    DagOperation *projected_ndc_depth, DagOperation *projected_linear_depth) {
    ResourceLoadingOption loading_option;
    loading_option.load_geometry = true;

    std::vector<DrawableInstance> observable_geometries =
        drawables->ObservableGeometries(projection, loading_option);

    GraphicsPipelineInterface *project_depth_pipeline = projected_ndc_depth->WithPipeline(
        kProjectDepthPipeline, [transfer_context](GraphicsPipelineOutputInterface *output) {
            return std::make_unique<ProjectDepthPipeline>(
                dynamic_cast<ProjectDepthOutput *>(output), transfer_context->vulkan_context);
        });

    auto args = std::make_unique<ProjectDepthArguments>(observable_geometries, projection,
                                                        transfer_context);
    projected_ndc_depth->Schedule(project_depth_pipeline, std::move(args),
                                  /*parents=*/std::vector<DagOperation *>{first_stage});

    if (projected_linear_depth == nullptr) {
        return;
    }

    GraphicsPipelineInterface *linearize_depth_pipeline = projected_linear_depth->WithPipeline(
        kLinearizeDepthPipeline,
        [transfer_context](GraphicsPipelineOutputInterface *linear_depth_output) {
            return std::make_unique<ScreenSpaceProcessorPipeline>(
                kLinearizeDepthPipeline, kFragmentShaderFilePathDepthMapLinearizerPerspective,
                /*input_image_count=*/1,
                /*push_constant_size=*/sizeof(LinearizeDepthPipelineParameters),
                linear_depth_output, transfer_context);
        });

    auto configurator = std::make_unique<LinearizeDepthPipelineConfigurator>(
        projection, *projected_ndc_depth->Output());
    projected_linear_depth->Schedule(linearize_depth_pipeline, std::move(configurator),
                                     /*parents=*/std::vector<DagOperation *>{projected_ndc_depth});
}

} // namespace e8
