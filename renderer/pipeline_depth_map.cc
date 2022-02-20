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
#include <cstddef>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/tensor.h"
#include "content/drawable.h"
#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_depth_map.h"
#include "renderer/projection.h"
#include "renderer/render_pass.h"
#include "renderer/vram.h"

namespace e8 {
namespace {

struct PushConstants {
    PushConstants(mat44 const &model_view_proj_trans)
        : model_view_proj_trans(model_view_proj_trans) {}

    mat44 model_view_proj_trans;
};

std::vector<VkVertexInputAttributeDescription> VertexShaderInputAttributes() {
    VkVertexInputAttributeDescription position_attribute;
    position_attribute.binding = 0;
    position_attribute.location = 0;
    position_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    position_attribute.offset = offsetof(PrimitiveVertex, position);

    return std::vector<VkVertexInputAttributeDescription>{position_attribute};
}

} // namespace

class DepthMapPipeline::DepthMapPipelineImpl {
  public:
    DepthMapPipelineImpl(unsigned width, unsigned height, VulkanContext *context);
    ~DepthMapPipelineImpl();

    ShaderUniformLayout const &GetUniformLayout() const;
    RenderPass const &GetRenderPass() const;
    GraphicsPipeline const &GetGraphicsPipeline() const;
    FrameBuffer const &GetFrameBuffer() const;
    FrameBufferAttachment const &GetDepthAttachment() const;

  public:
    VulkanContext *context;

  private:
    std::unique_ptr<ShaderStages> shader_stages_;
    std::unique_ptr<ShaderUniformLayout> uniform_layout_;
    std::unique_ptr<VertexInputInfo> vertex_inputs_;
    std::unique_ptr<FixedStageConfig> fixed_stage_config_;
    std::unique_ptr<FrameBufferAttachment> depth_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<GraphicsPipeline> pipeline_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

DepthMapPipeline::DepthMapPipelineImpl::DepthMapPipelineImpl(unsigned width, unsigned height,
                                                             VulkanContext *context)
    : context(context) {
    uniform_layout_ = CreateShaderUniformLayout(
        /*push_constant_size=*/sizeof(PushConstants),
        /*push_constant_accessible_stage=*/VK_SHADER_STAGE_VERTEX_BIT, context);
    shader_stages_ = CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathDepthMap,
                                        /*fragment_shader_file_path=*/std::nullopt, context);
    vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
    fixed_stage_config_ = CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                                 /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                                                 /*enable_depth_test=*/true, width, height);
    depth_attachment_ = CreateDepthAttachment(width, height, context);
    render_pass_ = CreateRenderPass(/*color_attachments=*/std::vector<VkAttachmentDescription>(),
                                    depth_attachment_->desc, context);

    pipeline_ = CreateGraphicsPipeline(*render_pass_, *shader_stages_, *uniform_layout_,
                                       *vertex_inputs_, *fixed_stage_config_, context);

    frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                      /*color_attachments=*/std::vector<VkImageView>(),
                                      depth_attachment_->view, context);
}

DepthMapPipeline::DepthMapPipelineImpl::~DepthMapPipelineImpl() {}

ShaderUniformLayout const &DepthMapPipeline::DepthMapPipelineImpl::GetUniformLayout() const {
    return *uniform_layout_;
}

RenderPass const &DepthMapPipeline::DepthMapPipelineImpl::GetRenderPass() const {
    return *render_pass_;
}

GraphicsPipeline const &DepthMapPipeline::DepthMapPipelineImpl::GetGraphicsPipeline() const {
    return *pipeline_;
}

FrameBuffer const &DepthMapPipeline::DepthMapPipelineImpl::GetFrameBuffer() const {
    return *frame_buffer_;
}

FrameBufferAttachment const &DepthMapPipeline::DepthMapPipelineImpl::GetDepthAttachment() const {
    return *depth_attachment_;
}

DepthMapPipeline::DepthMapPipeline(unsigned width, unsigned height, VulkanContext *context)
    : pimpl_(std::make_unique<DepthMapPipelineImpl>(width, height, context)) {}

DepthMapPipeline::~DepthMapPipeline() {}

DepthMapPipeline::FutureResult::FutureResult(FrameBufferAttachment const &depth_attachment,
                                             std::unique_ptr<GpuBarrier> &&barrier)
    : depth_attachment(depth_attachment), barrier(std::move(barrier)) {}

DepthMapPipeline::FutureResult::~FutureResult() {}

DepthMapPipeline::FutureResult DepthMapPipeline::Run(std::vector<DrawableInstance> const &drawables,
                                                     ProjectionInterface const &projection,
                                                     GpuBarrier const &barrier,
                                                     GeometryVramTransfer *geo_vram) {
    VkCommandBuffer cmds =
        StartRenderPass(pimpl_->GetRenderPass(), pimpl_->GetFrameBuffer(), pimpl_->context);

    ShaderUniformLayout const &uniform_layout = pimpl_->GetUniformLayout();
    RenderDrawables(
        drawables, pimpl_->GetGraphicsPipeline(),
        [&projection, &uniform_layout](DrawableInstance const &drawable, VkCommandBuffer cmds) {
            mat44 model_view_proj = projection.ProjectiveTransform() * projection.ViewTransform() *
                                    (*drawable.transform);
            PushConstants push_constants(model_view_proj);

            vkCmdPushConstants(cmds, uniform_layout.layout,
                               /*stageFlags=*/VK_SHADER_STAGE_VERTEX_BIT,
                               /*offset=*/0,
                               /*size=*/sizeof(PushConstants), &push_constants);
        },
        geo_vram, cmds);

    std::unique_ptr<GpuBarrier> this_barrier =
        FinishRenderPass(cmds, barrier, /*last=*/false, pimpl_->context);

    return FutureResult(pimpl_->GetDepthAttachment(), std::move(this_barrier));
}

} // namespace e8
