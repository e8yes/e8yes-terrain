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

#include <vulkan/vulkan.h>
#include <algorithm>
#include <memory>
#include <vector>

#include "common/device.h"
#include "content/common.h"
#include "content/scene.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/pipeline/project_surface.h"
#include "renderer/postprocessor/surface_projection_visualizer.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query/collection.h"
#include "renderer/renderer.h"
#include "renderer/renderer_surface_projection.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {

class SurfaceProjectionRenderer::SurfaceProjectionRendererImpl {
   public:
    SurfaceProjectionRendererImpl(std::unique_ptr<PipelineStage> &&final_color_image,
                                  VulkanContext *context);
    ~SurfaceProjectionRendererImpl();

    VulkanContext *context;
    DescriptorSetAllocator desc_set_alloc;
    TextureDescriptorSetCache tex_desc_set_cache;
    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;

    std::unique_ptr<PipelineStage> surface_projection;
    std::unique_ptr<PipelineStage> final_color_image;

    RendererConfiguration config;
};

SurfaceProjectionRenderer::SurfaceProjectionRendererImpl::SurfaceProjectionRendererImpl(
    std::unique_ptr<PipelineStage> &&final_color_image, VulkanContext *context)
    : context(context),
      desc_set_alloc(context),
      tex_desc_set_cache(&desc_set_alloc),
      geo_vram(context),
      tex_vram(context),
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

    PipelineStage *first_stage = this->DoFirstStage();
    DoProjectSurface(&drawable_collection, camera_projection, &pimpl_->tex_desc_set_cache,
                     &pimpl_->geo_vram, &pimpl_->tex_vram, pimpl_->context, first_stage,
                     pimpl_->surface_projection.get());
    DoVisualizeSurfaceProjection(pimpl_->config.light_inputs_renderer_params().input_to_visualize(),
                                 pimpl_->surface_projection.get(), &pimpl_->desc_set_alloc,
                                 pimpl_->context, pimpl_->final_color_image.get());
    PipelineStage *final_stage = this->DoFinalStage(first_stage, pimpl_->final_color_image.get());

    final_stage->Fulfill(pimpl_->context);
}

void SurfaceProjectionRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

}  // namespace e8
