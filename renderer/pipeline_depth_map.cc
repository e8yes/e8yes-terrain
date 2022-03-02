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
#include "content/proto/primitive.pb.h"
#include "renderer/context.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_depth_map.h"
#include "renderer/pipeline_output.h"
#include "renderer/projection.h"
#include "renderer/render_pass.h"
#include "renderer/vram_geometry.h"

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
    position_attribute.offset = kPrimitiveVertexPositionOffset;

    return std::vector<VkVertexInputAttributeDescription>{position_attribute};
}

} // namespace

class DepthMapPipeline::DepthMapPipelineImpl {
  public:
    DepthMapPipelineImpl(PipelineOutputInterface *output, VulkanContext *context);
    ~DepthMapPipelineImpl();

    ShaderUniformLayout const &GetUniformLayout() const;
    GraphicsPipeline const &GetGraphicsPipeline() const;

  public:
    PipelineOutputInterface *output;
    VulkanContext *context;

  private:
    std::unique_ptr<ShaderStages> shader_stages_;
    std::unique_ptr<ShaderUniformLayout> uniform_layout_;
    std::unique_ptr<VertexInputInfo> vertex_inputs_;
    std::unique_ptr<FixedStageConfig> fixed_stage_config_;

    std::unique_ptr<GraphicsPipeline> pipeline_;
};

DepthMapPipeline::DepthMapPipelineImpl::DepthMapPipelineImpl(PipelineOutputInterface *output,
                                                             VulkanContext *context)
    : output(output), context(context) {
    uniform_layout_ = CreateShaderUniformLayout(
        /*push_constant_size=*/sizeof(PushConstants),
        /*push_constant_accessible_stage=*/VK_SHADER_STAGE_VERTEX_BIT, context);
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

DepthMapPipeline::DepthMapPipeline(PipelineOutputInterface *output, VulkanContext *context)
    : pimpl_(std::make_unique<DepthMapPipelineImpl>(output, context)) {}

DepthMapPipeline::~DepthMapPipeline() {}

PipelineOutputInterface *DepthMapPipeline::Run(std::vector<DrawableInstance> const &drawables,
                                               ProjectionInterface const &projection,
                                               GpuBarrier const &barrier,
                                               GeometryVramTransfer *geo_vram) {
    VkCommandBuffer cmds = StartRenderPass(pimpl_->output->GetRenderPass(),
                                           *pimpl_->output->GetFrameBuffer(), pimpl_->context);

    ShaderUniformLayout const &uniform_layout = pimpl_->GetUniformLayout();
    RenderGeometrys(
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

    pimpl_->output->barrier = FinishRenderPass(cmds, barrier, /*last=*/false, pimpl_->context);

    return pimpl_->output;
}

} // namespace e8
