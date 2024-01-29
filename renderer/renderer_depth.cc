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
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "content/scene.h"
#include "renderer/basic/projection.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/drawable/collection.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/space_projection/project_depth.h"
#include "renderer/space_screen/depth_projection_visualizer.h"
#include "renderer/transfer/context.h"
#include "resource/accessor.h"

namespace e8 {

class DepthRenderer::DepthRendererImpl {
  public:
    DepthRendererImpl(VulkanContext *context);
    ~DepthRendererImpl();

    DagContext dag_context;
    TransferContext transfer_context;

    RendererConfiguration config;
};

DepthRenderer::DepthRendererImpl::DepthRendererImpl(VulkanContext *context)
    : dag_context(context), transfer_context(context) {}

DepthRenderer::DepthRendererImpl::~DepthRendererImpl() {}

DepthRenderer::DepthRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<DepthRendererImpl>(context)) {}

DepthRenderer::~DepthRenderer() {}

void DepthRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    Scene::ReadAccess read_access = scene->GainReadAccess();
    PerspectiveProjection projection(scene->camera);
    DrawableCollection drawables_collection(*scene->SceneEntityStructure(), resource_accessor);

    DagContext::Session session = pimpl_->dag_context.CreateSession();
    DagOperation *first_stage = this->DoFirstStage();

    DagOperationInstance ndc_depth_map =
        DoProjectNdcDepth(&drawables_collection, projection, first_stage, &pimpl_->transfer_context,
                          &pimpl_->dag_context);

    std::shared_ptr<GraphicsPipelineOutputInterface> final_color_output =
        RendererInterface::FinalColorImage();
    DagOperationInstance visualized_color_map = DoVisualizeDepthProjection(
        pimpl_->config.depth_renderer_params().alpha(), projection, ndc_depth_map,
        final_color_output, &pimpl_->transfer_context, &pimpl_->dag_context);

    DagOperation *final_stage = this->DoFinalStage(first_stage, visualized_color_map);

    final_stage->Fulfill(context);
}

void DepthRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
