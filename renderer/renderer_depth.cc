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
#include "renderer/pipeline/depth_map.h"
#include "renderer/postprocessor/depth_map_visualizer.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/query/query_fn.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {

class DepthRenderer::DepthRendererImpl {
  public:
    DepthRendererImpl(std::unique_ptr<PipelineStage> &&visual_representation,
                      VulkanContext *context);
    ~DepthRendererImpl();

    DescriptorSetAllocator desc_set_alloc;
    TextureDescriptorSetCache tex_desc_set_cache;
    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;

    std::unique_ptr<PipelineStage> depth_map;
    std::unique_ptr<PipelineStage> visual_representation;

    RendererConfiguration config;
};

DepthRenderer::DepthRendererImpl::DepthRendererImpl(
    std::unique_ptr<PipelineStage> &&visual_representation, VulkanContext *context)
    : desc_set_alloc(context), tex_desc_set_cache(&desc_set_alloc), geo_vram(context),
      tex_vram(context),
      depth_map(CreateDepthMapStage(context->swap_chain_image_extent.width,
                                    context->swap_chain_image_extent.height,
                                    context)),
      visual_representation(std::move(visual_representation)) {}

DepthRenderer::DepthRendererImpl::~DepthRendererImpl() {}

DepthRenderer::DepthRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<DepthRendererImpl>(RendererInterface::ColorMapStage(), context)) {}

DepthRenderer::~DepthRenderer() {}

void DepthRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    PerspectiveProjection camera_projection(scene->camera);
    std::vector<SceneEntity const *> scene_entities =
        scene->SceneEntityStructure()->QueryEntities(QueryAllSceneEntities);

    ResourceLoadingOption option;
    option.load_geometry = true;

    std::vector<DrawableInstance> drawables =
        ToDrawables(scene_entities, /*viewer_location=*/ToVec3(scene->camera.position()), option,
                    resource_accessor);

    PipelineStage *first_stage = this->DoFirstStage();
    DoDepthMapping(drawables, camera_projection, &pimpl_->tex_desc_set_cache, &pimpl_->geo_vram,
                   &pimpl_->tex_vram, context, first_stage, pimpl_->depth_map.get());
    DoVisualizeDepthMap(pimpl_->config.depth_renderer_params().alpha(), camera_projection,
                        pimpl_->depth_map.get(), &pimpl_->desc_set_alloc, context,
                        pimpl_->visual_representation.get());
    PipelineStage *final_stage = this->DoFinalStage(first_stage,
                                                    pimpl_->visual_representation.get());

    final_stage->Fulfill(context);
}

void DepthRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
