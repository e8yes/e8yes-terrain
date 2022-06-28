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
#include "renderer/output/pipeline_stage.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/postprocessor/light_inputs_visualizer.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/query/query_fn.h"
#include "renderer/renderer.h"
#include "renderer/renderer_light_inputs.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {

class LightInputsRenderer::LightInputsRendererImpl {
  public:
    LightInputsRendererImpl(std::unique_ptr<PipelineStage> &&final_color_image,
                            VulkanContext *context);
    ~LightInputsRendererImpl();

    VulkanContext *context;
    DescriptorSetAllocator desc_set_alloc;
    TextureDescriptorSetCache tex_desc_set_cache;
    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;

    std::unique_ptr<PipelineStage> light_inputs;
    std::unique_ptr<PipelineStage> final_color_image;

    RendererConfiguration config;
};

LightInputsRenderer::LightInputsRendererImpl::LightInputsRendererImpl(
    std::unique_ptr<PipelineStage> &&final_color_image, VulkanContext *context)
    : context(context), desc_set_alloc(context), tex_desc_set_cache(&desc_set_alloc),
      geo_vram(context), tex_vram(context),
      light_inputs(CreateLightInputsStage(context->swap_chain_image_extent.width,
                                          context->swap_chain_image_extent.height, context)),
      final_color_image(std::move(final_color_image)) {}

LightInputsRenderer::LightInputsRendererImpl::~LightInputsRendererImpl() {}

LightInputsRenderer::LightInputsRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<LightInputsRendererImpl>(RendererInterface::FinalColorImageStage(),
                                                       context)) {}

LightInputsRenderer::~LightInputsRenderer() {}

void LightInputsRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    PerspectiveProjection camera_projection(scene->camera);
    std::vector<SceneEntity const *> scene_entities =
        scene->SceneEntityStructure()->QueryEntities(QueryAllSceneEntities);

    ResourceLoadingOption option;
    option.load_geometry = true;
    option.load_material = true;

    std::vector<DrawableInstance> drawables =
        ToDrawables(scene_entities, /*viewer_location=*/ToVec3(scene->camera.position()), option,
                    resource_accessor);

    PipelineStage *first_stage = this->DoFirstStage();
    DoGenerateLightInputs(drawables, camera_projection, &pimpl_->tex_desc_set_cache,
                          &pimpl_->geo_vram, &pimpl_->tex_vram, pimpl_->context, first_stage,
                          pimpl_->light_inputs.get());
    DoVisualizeLightInputs(pimpl_->config.light_inputs_renderer_params().input_to_visualize(),
                           pimpl_->light_inputs.get(), &pimpl_->desc_set_alloc, pimpl_->context,
                           pimpl_->final_color_image.get());
    PipelineStage *final_stage = this->DoFinalStage(first_stage, pimpl_->final_color_image.get());

    final_stage->Fulfill(pimpl_->context);
}

void LightInputsRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
