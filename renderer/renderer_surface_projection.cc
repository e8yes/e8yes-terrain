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
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/drawable/collection.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_surface_projection.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/space_screen/surface_projection_visualizer.h"
#include "renderer/transfer/context.h"
#include "resource/accessor.h"

namespace e8 {

class SurfaceProjectionRenderer::SurfaceProjectionRendererImpl {
  public:
    SurfaceProjectionRendererImpl(VulkanContext *context);
    ~SurfaceProjectionRendererImpl();

    DagContext dag_context;
    TransferContext transfer_context;
    FrameResourceAllocator frame_resource_allocator;
    RendererConfiguration config;
};

SurfaceProjectionRenderer::SurfaceProjectionRendererImpl::SurfaceProjectionRendererImpl(
    VulkanContext *context)
    : dag_context(context), transfer_context(context), frame_resource_allocator(context) {}

SurfaceProjectionRenderer::SurfaceProjectionRendererImpl::~SurfaceProjectionRendererImpl() {}

SurfaceProjectionRenderer::SurfaceProjectionRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<SurfaceProjectionRendererImpl>(context)) {}

SurfaceProjectionRenderer::~SurfaceProjectionRenderer() {}

void SurfaceProjectionRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    std::shared_ptr<SwapChainOutput> render_output =
        this->AcquireFinalColorImage(&pimpl_->frame_resource_allocator);

    Scene::ReadAccess read_access = scene->GainReadAccess();
    PerspectiveProjection camera_projection(scene->camera);
    DrawableCollection drawable_collection(*scene->SceneEntityStructure(), resource_accessor);

    DagContext::Session session = pimpl_->dag_context.CreateSession();

    DagOperationInstance surface_projection =
        DoProjectSurface(&drawable_collection, camera_projection, render_output->Width(),
                         render_output->Height(), &session);
    DagOperationInstance visualized_surface = DoVisualizeSurfaceProjection(
        pimpl_->config.light_inputs_renderer_params().input_to_visualize(), surface_projection,
        render_output, &session);
    std::vector<GpuPromise *> final_waits =
        visualized_surface->Fulfill(/*wait=*/false, &pimpl_->frame_resource_allocator);
    this->PresentFinalColorImage(*render_output, final_waits);
}

void SurfaceProjectionRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
