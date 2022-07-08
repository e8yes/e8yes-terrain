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
#include <chrono>
#include <memory>
#include <vector>

#include "common/device.h"
#include "content/scene.h"
#include "renderer/lighting/direct_illuminator.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/pipeline/project_surface.h"
#include "renderer/pipeline/fill_color.h"
#include "renderer/postprocessor/exposure.h"
#include "renderer/postprocessor/fxaa.h"
#include "renderer/postprocessor/radiance.h"
#include "renderer/postprocessor/tone_map.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/query/light_source.h"
#include "renderer/query/query_fn.h"
#include "renderer/renderer.h"
#include "renderer/renderer_radiance.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {

struct RadianceRenderer::RadianceRendererImpl {
    RadianceRendererImpl(std::unique_ptr<PipelineStage> &&final_color_image,
                         VulkanContext *context);
    ~RadianceRendererImpl();

    VulkanContext *context;
    DescriptorSetAllocator desc_set_alloc;
    TextureDescriptorSetCache tex_desc_set_cache;
    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;

    std::unique_ptr<PipelineStage> surface_projection;
    DirectIlluminator direct_illuminator;
    std::unique_ptr<PipelineStage> log_luminance_map;
    std::unique_ptr<PipelineStage> exposure_value;
    std::unique_ptr<PipelineStage> ldr_image;
    std::unique_ptr<PipelineStage> final_color_image;

    RendererConfiguration config;
};

RadianceRenderer::RadianceRendererImpl::RadianceRendererImpl(
    std::unique_ptr<PipelineStage> &&final_color_image, VulkanContext *context)
    : context(context), desc_set_alloc(context), tex_desc_set_cache(&desc_set_alloc),
      geo_vram(context), tex_vram(context),
      surface_projection(CreateProjectSurfaceStage(context->swap_chain_image_extent.width,
                                          context->swap_chain_image_extent.height, context)),
      direct_illuminator(context->swap_chain_image_extent.width,
                         context->swap_chain_image_extent.height, context),
      log_luminance_map(CreateLogLuminaneStage(context->swap_chain_image_extent.width,
                                               context->swap_chain_image_extent.height, context)),
      exposure_value(CreateExposureStage(context)),
      ldr_image(CreateLdrImageStage(context->swap_chain_image_extent.width,
                                    context->swap_chain_image_extent.height, context)),
      final_color_image(std::move(final_color_image)) {}

RadianceRenderer::RadianceRendererImpl::~RadianceRendererImpl() {}

RadianceRenderer::RadianceRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<RadianceRendererImpl>(RendererInterface::FinalColorImageStage(),
                                                    context)) {}

RadianceRenderer::~RadianceRenderer() {}

void RadianceRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    std::vector<SceneEntity const *> scene_entities =
        scene->SceneEntityStructure()->QueryEntities(QueryAllSceneEntities);

    // Loads drawables to the host memory.
    ResourceLoadingOption option;
    option.load_geometry = true;
    option.load_material = true;

    std::vector<DrawableInstance> drawables =
        ToDrawables(scene_entities, /*viewer_location=*/ToVec3(scene->camera.position()), option,
                    resource_accessor);

    // Render passes.
    PerspectiveProjection camera_projection(scene->camera);

    PipelineStage *first_stage = this->DoFirstStage();

    DoProjectSurface(drawables, camera_projection, &pimpl_->tex_desc_set_cache,
                          &pimpl_->geo_vram, &pimpl_->tex_vram, pimpl_->context, first_stage,
                          pimpl_->surface_projection.get());

    std::vector<LightSourceInstance> light_sources =
        ToLightSources(scene_entities, camera_projection);
    PipelineStage *radiance_map = pimpl_->direct_illuminator.DoComputeDirectIllumination(
        light_sources, camera_projection, pimpl_->surface_projection.get(), first_stage,
        &pimpl_->desc_set_alloc);

    DoEstimateExposure(radiance_map, &pimpl_->desc_set_alloc, pimpl_->context,
                       pimpl_->log_luminance_map.get(), pimpl_->exposure_value.get());
    DoToneMapping(radiance_map, pimpl_->exposure_value.get(), &pimpl_->desc_set_alloc,
                  pimpl_->context, pimpl_->ldr_image.get());
    DoFxaa(pimpl_->ldr_image.get(), &pimpl_->desc_set_alloc, pimpl_->context,
           pimpl_->final_color_image.get());

    PipelineStage *final_stage = this->DoFinalStage(
        first_stage, pimpl_->final_color_image.get(),
        /*dangling_stages=*/std::vector<PipelineStage *>{pimpl_->surface_projection.get()});

    final_stage->Fulfill(pimpl_->context);
}

void RadianceRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
