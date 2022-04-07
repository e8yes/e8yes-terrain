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
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/basic/pipeline_common.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/pass/render_pass.h"
#include "renderer/pipeline/depth_map.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"

namespace e8 {
namespace {

struct PushConstants {
    PushConstants(mat44 const &model_view_proj_trans)
        : model_view_proj_trans(model_view_proj_trans) {}

    mat44 model_view_proj_trans;
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

    return std::vector<VkVertexInputAttributeDescription>{position_attribute};
}

class RenderPassConfigurator : public RenderPassConfiguratorInterface {
  public:
    RenderPassConfigurator(ProjectionInterface const &projection,
                           ShaderUniformLayout const &uniform_layout);
    ~RenderPassConfigurator();

    void PushConstant(DrawableInstance const &drawable, VkCommandBuffer cmds) const override;

  private:
    ProjectionInterface const &projection_;
    ShaderUniformLayout const &uniform_layout_;
};

RenderPassConfigurator::RenderPassConfigurator(ProjectionInterface const &projection,
                                               ShaderUniformLayout const &uniform_layout)
    : projection_(projection), uniform_layout_(uniform_layout) {}

RenderPassConfigurator::~RenderPassConfigurator() {}

void RenderPassConfigurator::PushConstant(DrawableInstance const &drawable,
                                          VkCommandBuffer cmds) const {
    mat44 model_view_proj =
        projection_.ProjectiveTransform() * projection_.ViewTransform() * (*drawable.transform);
    PushConstants push_constants(model_view_proj);

    vkCmdPushConstants(cmds, uniform_layout_.layout,
                       /*stageFlags=*/VK_SHADER_STAGE_VERTEX_BIT,
                       /*offset=*/0,
                       /*size=*/sizeof(PushConstants), &push_constants);
}

} // namespace

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
    : PipelineOutputInterface(context),
      pimpl_(std::make_unique<DepthMapPipelineOutputImpl>(width, height, context)) {
    this->width = width;
    this->height = height;
}

DepthMapPipelineOutput::~DepthMapPipelineOutput() {}

FrameBuffer *DepthMapPipelineOutput::GetFrameBuffer() const { return pimpl_->frame_buffer_.get(); }

RenderPass const &DepthMapPipelineOutput::GetRenderPass() const { return *pimpl_->render_pass_; }

FrameBufferAttachment const *DepthMapPipelineOutput::ColorAttachment() const { return nullptr; }

FrameBufferAttachment const *DepthMapPipelineOutput::DepthAttachment() const {
    return pimpl_->depth_attachment_.get();
}

class DepthMapPipeline::DepthMapPipelineImpl {
  public:
    DepthMapPipelineImpl(DepthMapPipelineOutput *output, VulkanContext *context);
    ~DepthMapPipelineImpl();

    ShaderUniformLayout const &GetUniformLayout() const;
    GraphicsPipeline const &GetGraphicsPipeline() const;

  public:
    DepthMapPipelineOutput *output;
    VulkanContext *context;

  private:
    std::unique_ptr<ShaderStages> shader_stages_;
    std::unique_ptr<ShaderUniformLayout> uniform_layout_;
    std::unique_ptr<VertexInputInfo> vertex_inputs_;
    std::unique_ptr<FixedStageConfig> fixed_stage_config_;

    std::unique_ptr<GraphicsPipeline> pipeline_;
};

DepthMapPipeline::DepthMapPipelineImpl::DepthMapPipelineImpl(DepthMapPipelineOutput *output,
                                                             VulkanContext *context)
    : output(output), context(context) {
    uniform_layout_ = CreateShaderUniformLayout(
        PushConstantLayout(), /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
    shader_stages_ = CreateShaderStages(/*vertex_shader_file_path=*/kVertexShaderFilePathDepthMap,
                                        /*fragment_shader_file_path=*/std::nullopt, context);
    vertex_inputs_ = CreateVertexInputState(VertexShaderInputAttributes());
    fixed_stage_config_ =
        CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                               /*cull_mode=*/VK_CULL_MODE_BACK_BIT,
                               /*enable_depth_test=*/true, output->width, output->height);

    pipeline_ = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                       *vertex_inputs_, *fixed_stage_config_, context);
}

DepthMapPipeline::DepthMapPipelineImpl::~DepthMapPipelineImpl() {}

ShaderUniformLayout const &DepthMapPipeline::DepthMapPipelineImpl::GetUniformLayout() const {
    return *uniform_layout_;
}

GraphicsPipeline const &DepthMapPipeline::DepthMapPipelineImpl::GetGraphicsPipeline() const {
    return *pipeline_;
}

DepthMapPipeline::DepthMapPipeline(DepthMapPipelineOutput *output, VulkanContext *context)
    : pimpl_(std::make_unique<DepthMapPipelineImpl>(output, context)) {}

DepthMapPipeline::~DepthMapPipeline() {}

DepthMapPipelineOutput *DepthMapPipeline::Run(std::vector<DrawableInstance> const &drawables,
                                              ProjectionInterface const &projection,
                                              GpuBarrier const &prerequisites,
                                              TextureDescriptorSetCache *tex_desc_set_cache,
                                              GeometryVramTransfer *geo_vram,
                                              TextureVramTransfer *tex_vram) {
    VkCommandBuffer cmds = StartRenderPass(pimpl_->output->GetRenderPass(),
                                           *pimpl_->output->GetFrameBuffer(), pimpl_->context);

    RenderPassConfigurator configurator(projection, pimpl_->GetUniformLayout());
    RenderDrawables(drawables, pimpl_->GetGraphicsPipeline(), pimpl_->GetUniformLayout(),
                    configurator, tex_desc_set_cache, geo_vram, tex_vram, cmds);

    pimpl_->output->barrier =
        FinishRenderPass(cmds, prerequisites, pimpl_->output->AcquireFence(), pimpl_->context);

    return pimpl_->output;
}

} // namespace e8
