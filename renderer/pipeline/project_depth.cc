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
#include "content/structure.h"
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
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/pass/configurator.h"
#include "renderer/pass/rasterize.h"
#include "renderer/pipeline/project_depth.h"
#include "renderer/query/collection.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {
namespace {

PipelineKey kProjectDepthPipeline = "Project Depth";

/**
 * @brief The ProjectDepthPipelineOutput class For storing, depth-only rendering output. The depth
 * values are stored in 32-bit floats.
 */
class ProjectDepthPipelineOutput : public PipelineOutputInterface {
  public:
    /**
     * @brief ProjectDepthPipelineOutput Constructs a depth map output with the specified dimension.
     *
     * @param width The width of the depth map output.
     * @param height The height of the depth map output.
     * @param context Contextual Vulkan handles.
     */
    ProjectDepthPipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~ProjectDepthPipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct DepthMapPipelineOutputImpl;
    std::unique_ptr<DepthMapPipelineOutputImpl> pimpl_;
};

struct ProjectDepthPipelineOutput::DepthMapPipelineOutputImpl {
    DepthMapPipelineOutputImpl(unsigned width, unsigned height, VulkanContext *context);
    ~DepthMapPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;

    VulkanContext *context;
};

ProjectDepthPipelineOutput::DepthMapPipelineOutputImpl::DepthMapPipelineOutputImpl(
    unsigned width, unsigned height, VulkanContext *context) {
    depth_attachment_ = CreateDepthAttachment(width, height, /*samplable=*/true, context);
    render_pass_ = CreateRenderPass(/*color_attachments=*/std::vector<VkAttachmentDescription>(),
                                    depth_attachment_->desc, context);
    frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                      /*color_attachments=*/std::vector<VkImageView>(),
                                      depth_attachment_->view, context);
}

ProjectDepthPipelineOutput::DepthMapPipelineOutputImpl::~DepthMapPipelineOutputImpl() {}

ProjectDepthPipelineOutput::ProjectDepthPipelineOutput(unsigned width, unsigned height,
                                                       VulkanContext *context)
    : PipelineOutputInterface(width, height),
      pimpl_(std::make_unique<DepthMapPipelineOutputImpl>(width, height, context)) {}

ProjectDepthPipelineOutput::~ProjectDepthPipelineOutput() {}

FrameBuffer *ProjectDepthPipelineOutput::GetFrameBuffer() const {
    return pimpl_->frame_buffer_.get();
}

RenderPass const &ProjectDepthPipelineOutput::GetRenderPass() const {
    return *pimpl_->render_pass_;
}

std::vector<FrameBufferAttachment const *> ProjectDepthPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{};
}

FrameBufferAttachment const *ProjectDepthPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
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

class RenderPassConfigurator : public RenderPassConfiguratorInterface {
  public:
    RenderPassConfigurator(ProjectionInterface const &projection);
    ~RenderPassConfigurator();

    std::vector<uint8_t> PushConstantOf(DrawableInstance const &drawable) const override;

  private:
    ProjectionInterface const &projection_;
};

RenderPassConfigurator::RenderPassConfigurator(ProjectionInterface const &projection)
    : projection_(projection) {}

RenderPassConfigurator::~RenderPassConfigurator() {}

std::vector<uint8_t>
RenderPassConfigurator::PushConstantOf(DrawableInstance const &drawable) const {
    std::vector<uint8_t> bytes(sizeof(PushConstant));

    PushConstant *push_constant = reinterpret_cast<PushConstant *>(bytes.data());
    push_constant->model_view_proj_trans =
        projection_.ProjectiveTransform() * projection_.ViewTransform() * (*drawable.transform);

    return bytes;
}

struct ProjectDepthPipelineArguments : public CachedPipelineArgumentsInterface {
    ProjectDepthPipelineArguments(std::vector<DrawableInstance> const &drawables,
                                  ProjectionInterface const &projection,
                                  TransferContext *transfer_context)
        : drawables(drawables), projection(projection), transfer_context(transfer_context) {}

    std::vector<DrawableInstance> drawables;
    ProjectionInterface const &projection;
    TransferContext *transfer_context;
};

class ProjectDepthPipeline : public CachedPipelineInterface {
  public:
    ProjectDepthPipeline(ProjectDepthPipelineOutput *output, VulkanContext *context);
    ~ProjectDepthPipeline() override;

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;
};

ProjectDepthPipeline::ProjectDepthPipeline(ProjectDepthPipelineOutput *output,
                                           VulkanContext *context)
    : CachedPipelineInterface(context) {
    uniform_layout_ = CreateShaderUniformLayout(
        PushConstantLayout(), /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
    shader_stages_ = CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathDepthMap,
                                        /*fragment_shader_file_path=*/std::nullopt, context);
    vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
    fixed_stage_config_ = CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                                 /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                                 /*enable_depth_test=*/true, output->Width(),
                                                 output->Height(), /*color_attachment_count=*/0);

    pipeline_ = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                       *vertex_inputs_, *fixed_stage_config_, context);
}

ProjectDepthPipeline::~ProjectDepthPipeline() {}

PipelineKey ProjectDepthPipeline::Key() const { return kProjectDepthPipeline; }

Fulfillment ProjectDepthPipeline::Launch(CachedPipelineArgumentsInterface const &generic_args,
                                         std::vector<GpuPromise *> const &prerequisites,
                                         unsigned completion_signal_count,
                                         PipelineOutputInterface *output) {
    ProjectDepthPipelineArguments const &args =
        static_cast<ProjectDepthPipelineArguments const &>(generic_args);

    VkCommandBuffer cmds =
        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), context_);

    RenderPassConfigurator configurator(args.projection);
    RenderDrawables(args.drawables, *pipeline_, *uniform_layout_, configurator,
                    args.transfer_context, cmds);

    return FinishRenderPass(cmds, completion_signal_count, prerequisites, context_);
}

} // namespace

std::unique_ptr<PipelineStage> CreateProjectDepthStage(unsigned width, unsigned height,
                                                       VulkanContext *context) {
    auto output = std::make_shared<ProjectDepthPipelineOutput>(width, height, context);
    return std::make_unique<PipelineStage>(output);
}

void DoProjectDepth(DrawableCollection *drawables, PerspectiveProjection const &projection,
                    TransferContext *transfer_context, PipelineStage *first_stage,
                    PipelineStage *target) {
    ResourceLoadingOption loading_option;
    loading_option.load_geometry = true;

    std::vector<DrawableInstance> observable_geometries =
        drawables->ObservableGeometries(projection, loading_option);

    CachedPipelineInterface *pipeline = target->WithPipeline(
        kProjectDepthPipeline, [transfer_context](PipelineOutputInterface *output) {
            return std::make_unique<ProjectDepthPipeline>(
                dynamic_cast<ProjectDepthPipelineOutput *>(output),
                transfer_context->vulkan_context);
        });

    auto args = std::make_unique<ProjectDepthPipelineArguments>(observable_geometries, projection,
                                                                transfer_context);
    target->Schedule(pipeline, std::move(args),
                     /*parents=*/std::vector<PipelineStage *>{first_stage});
}

} // namespace e8
