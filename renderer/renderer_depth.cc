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
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_depth_map.h"
#include "renderer/pipeline_depth_map_visualizer.h"
#include "renderer/pipeline_output.h"
#include "renderer/projection.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/query_fn.h"
#include "renderer/render_pass.h"
#include "renderer/renderer.h"
#include "renderer/renderer_depth.h"
#include "renderer/vram_geometry.h"
#include "renderer/vram_texture.h"
#include "resource/accessor.h"

namespace e8 {

class DepthRenderer::DepthRendererImpl {
  public:
    DepthRendererImpl(VulkanContext *context);
    ~DepthRendererImpl();

    DepthMapPipelineOutput depth_map_output;
    SwapChainPipelineOutput final_output;

    DepthMapPipeline depth_map_pipeline;
    DepthMapVisualizerPipeline depth_map_visualizer_pipeline;

    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;
    RendererConfiguration config;
};

DepthRenderer::DepthRendererImpl::DepthRendererImpl(VulkanContext *context)
    : depth_map_output(context->swap_chain_image_extent.width,
                       context->swap_chain_image_extent.height, context),
      final_output(/*with_depth_buffer=*/false, context),
      depth_map_pipeline(&depth_map_output, context),
      depth_map_visualizer_pipeline(&final_output, context), geo_vram(context), tex_vram(context) {}

DepthRenderer::DepthRendererImpl::~DepthRendererImpl() {}

DepthRenderer::DepthRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, /*max_frame_duration=*/std::chrono::seconds(10), context),
      pimpl_(std::make_unique<DepthRendererImpl>(context)) {}

DepthRenderer::~DepthRenderer() {}

void DepthRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    FrameContext frame_context = this->BeginFrame();

    pimpl_->final_output.SetSwapChainImageIndex(frame_context.swap_chain_image_index);

    PipelineOutputInterface *final_output;

    this->BeginStage(/*index=*/1, /*name=*/"Pipeline Preparation");
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();

        PerspectiveProjection camera_projection(scene->camera);
        std::vector<SceneEntity const *> scene_entities =
            scene->SceneEntityStructure()->QueryEntities(QueryAllSceneEntities);

        ResourceLoadingOption option;
        option.load_geometry = true;

        std::vector<DrawableInstance> drawables =
            ToDrawables(scene_entities, /*viewer_location=*/ToVec3(scene->camera.position()),
                        option, resource_accessor);

        DepthMapPipelineOutput *depth_map_output = pimpl_->depth_map_pipeline.Run(
            drawables, camera_projection, frame_context.acquire_swap_chain_image_barrier,
            &pimpl_->geo_vram, &pimpl_->tex_vram);

        final_output = pimpl_->depth_map_visualizer_pipeline.Run(
            pimpl_->config.depth_renderer_params().alpha(), camera_projection, *depth_map_output);
    }
    this->EndStage(/*index=*/1);

    this->EndFrame(frame_context, final_output);
}

void DepthRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
