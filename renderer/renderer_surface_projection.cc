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
#include "renderer/dag/dag_operation.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query/collection.h"
#include "renderer/renderer.h"
#include "renderer/renderer_surface_projection.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/space_screen/surface_projection_visualizer.h"
#include "renderer/transfer/context.h"
#include "resource/accessor.h"

namespace e8 {

class SurfaceProjectionRenderer::SurfaceProjectionRendererImpl {
  public:
    SurfaceProjectionRendererImpl(std::unique_ptr<DagOperation> &&final_color_image,
                                  VulkanContext *context);
    ~SurfaceProjectionRendererImpl();

    TransferContext transfer_context;

    std::unique_ptr<DagOperation> surface_projection;
    std::unique_ptr<DagOperation> final_color_image;

    RendererConfiguration config;
};

SurfaceProjectionRenderer::SurfaceProjectionRendererImpl::SurfaceProjectionRendererImpl(
    std::unique_ptr<DagOperation> &&final_color_image, VulkanContext *context)
    : transfer_context(context),
      surface_projection(CreateProjectSurfaceStage(context->swap_chain_image_extent.width,
                                                   context->swap_chain_image_extent.height,
                                                   context)),
      final_color_image(std::move(final_color_image)) {}

SurfaceProjectionRenderer::SurfaceProjectionRendererImpl::~SurfaceProjectionRendererImpl() {}

SurfaceProjectionRenderer::SurfaceProjectionRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<SurfaceProjectionRendererImpl>(
          RendererInterface::FinalColorImageStage(), context)) {}

SurfaceProjectionRenderer::~SurfaceProjectionRenderer() {}

void SurfaceProjectionRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    PerspectiveProjection camera_projection(scene->camera);
    DrawableCollection drawable_collection(*scene->SceneEntityStructure(), resource_accessor);

    DagOperation *first_stage = this->DoFirstStage();
    DoProjectSurface(&drawable_collection, camera_projection, &pimpl_->transfer_context,
                     first_stage, pimpl_->surface_projection.get());
    DoVisualizeSurfaceProjection(pimpl_->config.light_inputs_renderer_params().input_to_visualize(),
                                 pimpl_->surface_projection.get(), &pimpl_->transfer_context,
                                 pimpl_->final_color_image.get());
    DagOperation *final_stage = this->DoFinalStage(first_stage, pimpl_->final_color_image.get());

    final_stage->Fulfill(pimpl_->transfer_context.vulkan_context);
}

void SurfaceProjectionRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
