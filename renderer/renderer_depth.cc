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
#include "content/common.h"
#include "content/scene.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/space_projection/project_depth.h"
#include "renderer/space_screen/depth_projection_visualizer.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query/collection.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/transfer/context.h"
#include "resource/accessor.h"

namespace e8 {

class DepthRenderer::DepthRendererImpl {
  public:
    DepthRendererImpl(std::unique_ptr<DagOperation> &&visual_representation,
                      VulkanContext *context);
    ~DepthRendererImpl();

    TransferContext transfer_context;

    std::unique_ptr<DagOperation> depth_projection;
    std::unique_ptr<DagOperation> visual_representation;

    RendererConfiguration config;
};

DepthRenderer::DepthRendererImpl::DepthRendererImpl(
    std::unique_ptr<DagOperation> &&visual_representation, VulkanContext *context)
    : transfer_context(context),
      depth_projection(CreateProjectNdcDepthStage(context->swap_chain_image_extent.width,
                                                  context->swap_chain_image_extent.height,
                                                  context)),
      visual_representation(std::move(visual_representation)) {}

DepthRenderer::DepthRendererImpl::~DepthRendererImpl() {}

DepthRenderer::DepthRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<DepthRendererImpl>(RendererInterface::FinalColorImageStage(),
                                                 context)) {}

DepthRenderer::~DepthRenderer() {}

void DepthRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    PerspectiveProjection projection(scene->camera);
    DrawableCollection drawables_collection(*scene->SceneEntityStructure(), resource_accessor);

    DagOperation *first_stage = this->DoFirstStage();
    DoProjectDepth(&drawables_collection, projection, &pimpl_->transfer_context, first_stage,
                   pimpl_->depth_projection.get());
    DoVisualizeDepthProjection(pimpl_->config.depth_renderer_params().alpha(), projection,
                               pimpl_->depth_projection.get(), &pimpl_->transfer_context,
                               pimpl_->visual_representation.get());
    DagOperation *final_stage =
        this->DoFinalStage(first_stage, pimpl_->visual_representation.get());

    final_stage->Fulfill(context);
}

void DepthRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
