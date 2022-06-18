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
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pass/configurator.h"
#include "renderer/pass/rasterize.h"
#include "renderer/pipeline/depth_map.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"

namespace e8 {
namespace {

PipelineStage::PipelineKey kDepthMapPipeline = "DepthMap";

/**
 * @brief The DepthMapPipelineOutput class For storing, depth-only rendering output. The depth
 * values are stored in 32-bit floats.
 */
class DepthMapPipelineOutput : public PipelineOutputInterface {
  public:
    /**
     * @brief DepthMapPipelineOutput Constructs a depth map output with the specified dimension.
     *
     * @param width The width of the depth map output.
     * @param height The height of the depth map output.
     * @param context Contextual Vulkan handles.
     */
    DepthMapPipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~DepthMapPipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct DepthMapPipelineOutputImpl;
    std::unique_ptr<DepthMapPipelineOutputImpl> pimpl_;
};

struct DepthMapPipelineOutput::DepthMapPipelineOutputImpl {
    DepthMapPipelineOutputImpl(unsigned width, unsigned height, VulkanContext *context);
    ~DepthMapPipelineOutputImpl();

    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;

    VulkanContext *context;
};

DepthMapPipelineOutput::DepthMapPipelineOutputImpl::DepthMapPipelineOutputImpl(
    unsigned width, unsigned height, VulkanContext *context) {
    depth_attachment_ = CreateDepthAttachment(width, height, /*samplable=*/true, context);
    render_pass_ = CreateRenderPass(/*color_attachments=*/std::vector<VkAttachmentDescription>(),
                                    depth_attachment_->desc, context);
    frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                      /*color_attachments=*/std::vector<VkImageView>(),
                                      depth_attachment_->view, context);
}

DepthMapPipelineOutput::DepthMapPipelineOutputImpl::~DepthMapPipelineOutputImpl() {}

DepthMapPipelineOutput::DepthMapPipelineOutput(unsigned width, unsigned height,
                                               VulkanContext *context)
    : PipelineOutputInterface(width, height, context),
      pimpl_(std::make_unique<DepthMapPipelineOutputImpl>(width, height, context)) {}

DepthMapPipelineOutput::~DepthMapPipelineOutput() {}

FrameBuffer *DepthMapPipelineOutput::GetFrameBuffer() const { return pimpl_->frame_buffer_.get(); }

RenderPass const &DepthMapPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass_; }

std::vector<FrameBufferAttachment const *> DepthMapPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{};
}

FrameBufferAttachment const *DepthMapPipelineOutput::DepthAttachment() const {
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

class DepthMapPipeline : public CachedPipelineInterface {
  public:
    DepthMapPipeline(DepthMapPipelineOutput *output, VulkanContext *context);
    ~DepthMapPipeline() override;

    Fulfillment Launch(unsigned instance_id, std::vector<PipelineOutputInterface *> const &inputs,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;

    void SetDrawables(std::vector<DrawableInstance> const &drawables);
    void SetProjection(ProjectionInterface const &projection);
    void SetTransferCache(TextureDescriptorSetCache *tex_desc_set_cache,
                          GeometryVramTransfer *geo_vram, TextureVramTransfer *tex_vram);

  private:
    std::vector<DrawableInstance> const *drawables_;
    ProjectionInterface const *projection_;
    TextureDescriptorSetCache *tex_desc_set_cache_;
    GeometryVramTransfer *geo_vram_;
    TextureVramTransfer *tex_vram_;
};

DepthMapPipeline::DepthMapPipeline(DepthMapPipelineOutput *output, VulkanContext *context)
    : CachedPipelineInterface(context), drawables_(nullptr), projection_(nullptr),
      tex_desc_set_cache_(nullptr), geo_vram_(nullptr), tex_vram_(nullptr) {
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

DepthMapPipeline::~DepthMapPipeline() {}

Fulfillment DepthMapPipeline::Launch(unsigned /*instance_id*/,
                                     std::vector<PipelineOutputInterface *> const & /*inputs*/,
                                     std::vector<GpuPromise *> const &prerequisites,
                                     unsigned completion_signal_count,
                                     PipelineOutputInterface *output) {
    assert(drawables_ != nullptr);
    assert(projection_ != nullptr);
    assert(tex_desc_set_cache_ != nullptr);
    assert(geo_vram_ != nullptr);
    assert(tex_vram_ != nullptr);

    VkCommandBuffer cmds =
        StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), context_);

    RenderPassConfigurator configurator(*projection_);
    RenderDrawables(*drawables_, *pipeline_, *uniform_layout_, configurator, tex_desc_set_cache_,
                    geo_vram_, tex_vram_, cmds);

    return FinishRenderPass2(cmds, completion_signal_count, prerequisites, context_);
}

void DepthMapPipeline::SetDrawables(std::vector<DrawableInstance> const &drawables) {
    drawables_ = &drawables;
}

void DepthMapPipeline::SetProjection(ProjectionInterface const &projection) {
    projection_ = &projection;
}

void DepthMapPipeline::SetTransferCache(TextureDescriptorSetCache *tex_desc_set_cache,
                                        GeometryVramTransfer *geo_vram,
                                        TextureVramTransfer *tex_vram) {
    tex_desc_set_cache_ = tex_desc_set_cache;
    geo_vram_ = geo_vram;
    tex_vram_ = tex_vram;
}

} // namespace

void DoDepthMapping(std::vector<DrawableInstance> const &drawables,
                    ProjectionInterface const &projection,
                    TextureDescriptorSetCache *tex_desc_set_cache, GeometryVramTransfer *geo_vram,
                    TextureVramTransfer *tex_vram, VulkanContext *context,
                     PipelineStage* first_stage, PipelineStage *target) {
    DepthMapPipeline *pipeline = static_cast<DepthMapPipeline *>(
        target->WithPipeline(kDepthMapPipeline, [context](PipelineOutputInterface *output) {
            return std::make_unique<DepthMapPipeline>(
                dynamic_cast<DepthMapPipelineOutput *>(output), context);
        }));

    pipeline->SetDrawables(drawables);
    pipeline->SetProjection(projection);
    pipeline->SetTransferCache(tex_desc_set_cache, geo_vram, tex_vram);

    target->Schedule(kDepthMapPipeline, /*parents=*/std::vector<PipelineStage *>{first_stage},
                     /*instance_count=*/1);
}

} // namespace e8
