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
#include "renderer/output/common_output.h"
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
    LightInputsRendererImpl(VulkanContext *context);
    ~LightInputsRendererImpl();

    LightInputsPipelineOutput light_inputs_map;
    SwapChainPipelineOutput color_map;

    DescriptorSetAllocator desc_set_alloc;
    TextureDescriptorSetCache tex_desc_set_cache;
    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;

    LightInputsPipeline light_inputs_pipeline;
    LightInputsVisualizerPipeline light_inputs_visualizer_pipeline;

    RendererConfiguration config;
};

LightInputsRenderer::LightInputsRendererImpl::LightInputsRendererImpl(VulkanContext *context)
    : light_inputs_map(context->swap_chain_image_extent.width,
                       context->swap_chain_image_extent.height, context),
      color_map(/*with_depth_buffer=*/false, context), desc_set_alloc(context),
      tex_desc_set_cache(&desc_set_alloc), geo_vram(context), tex_vram(context),
      light_inputs_pipeline(context), light_inputs_visualizer_pipeline(&desc_set_alloc, context) {}

LightInputsRenderer::LightInputsRendererImpl::~LightInputsRendererImpl() {}

LightInputsRenderer::LightInputsRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<LightInputsRendererImpl>(context)) {}

LightInputsRenderer::~LightInputsRenderer() {}

void LightInputsRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    FrameContext frame_context = this->BeginFrame();

    pimpl_->color_map.SetSwapChainImageIndex(frame_context.swap_chain_image_index);

    this->BeginStage(/*index=*/1, /*name=*/"Pipeline Preparation");
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();

        PerspectiveProjection camera_projection(scene->camera);
        std::vector<SceneEntity const *> scene_entities =
            scene->SceneEntityStructure()->QueryEntities(QueryAllSceneEntities);

        ResourceLoadingOption option;
        option.load_geometry = true;
        option.load_material = true;

        std::vector<DrawableInstance> drawables =
            ToDrawables(scene_entities, /*viewer_location=*/ToVec3(scene->camera.position()),
                        option, resource_accessor);

        pimpl_->light_inputs_pipeline.Run(drawables, camera_projection,
                                          frame_context.swap_chain_image_promise,
                                          &pimpl_->tex_desc_set_cache, &pimpl_->geo_vram,
                                          &pimpl_->tex_vram, &pimpl_->light_inputs_map);

        pimpl_->light_inputs_visualizer_pipeline.Run(
            pimpl_->config.light_inputs_renderer_params().input_to_visualize(),
            pimpl_->light_inputs_map, &pimpl_->color_map);
    }
    this->EndStage(/*index=*/1);

    this->EndFrame(frame_context, std::vector<PipelineOutputInterface *>{
                                      &pimpl_->light_inputs_map,
                                      &pimpl_->color_map,
                                  });
}

void LightInputsRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
