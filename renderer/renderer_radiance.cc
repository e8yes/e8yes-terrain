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
#include "renderer/output/pipeline_output.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/postprocessor/radiance_directional.h"
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
    RadianceRendererImpl(VulkanContext *context);
    ~RadianceRendererImpl();

    LightInputsPipelineOutput light_inputs_output;
    UnboundedColorPipelineOutput radiance_output;
    SwapChainPipelineOutput final_output;

    DescriptorSetAllocator desc_set_alloc;
    TextureDescriptorSetCache tex_desc_set_cache;
    GeometryVramTransfer geo_vram;
    TextureVramTransfer tex_vram;

    LightInputsPipeline light_inputs_pipeline;
    DirectionalRadiancePipeline directional_radiance_pipeline;
    ToneMapPipeline tone_map_pipeline;

    RendererConfiguration config;
};

RadianceRenderer::RadianceRendererImpl::RadianceRendererImpl(VulkanContext *context)
    : light_inputs_output(context->swap_chain_image_extent.width,
                          context->swap_chain_image_extent.height, context),
      radiance_output(context->swap_chain_image_extent.width,
                      context->swap_chain_image_extent.height, /*with_depth_buffer=*/false,
                      context),
      final_output(/*with_depth_buffer=*/false, context), desc_set_alloc(context),
      tex_desc_set_cache(&desc_set_alloc), geo_vram(context), tex_vram(context),
      light_inputs_pipeline(&light_inputs_output, context),
      directional_radiance_pipeline(&radiance_output, &desc_set_alloc, context),
      tone_map_pipeline(&final_output, &desc_set_alloc, context) {}

RadianceRenderer::RadianceRendererImpl::~RadianceRendererImpl() {}

RadianceRenderer::RadianceRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, /*max_frame_duration=*/std::chrono::seconds(10), context),
      pimpl_(std::make_unique<RadianceRendererImpl>(context)) {}

RadianceRenderer::~RadianceRenderer() {}

void RadianceRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    FrameContext frame_context = this->BeginFrame();

    pimpl_->final_output.SetSwapChainImageIndex(frame_context.swap_chain_image_index);

    PipelineOutputInterface *final_output;

    this->BeginStage(/*index=*/1, /*name=*/"Pipeline Preparation");
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();

        PerspectiveProjection camera_projection(scene->camera);
        std::vector<SceneEntity const *> scene_entities =
            scene->SceneEntityStructure()->QueryEntities(QueryAllSceneEntities);

        // Loads drawables to the host memory.
        ResourceLoadingOption option;
        option.load_geometry = true;
        option.load_material = true;

        std::vector<DrawableInstance> drawables =
            ToDrawables(scene_entities, /*viewer_location=*/ToVec3(scene->camera.position()),
                        option, resource_accessor);

        // Render passes.
        LightInputsPipelineOutput *light_inputs = pimpl_->light_inputs_pipeline.Run(
            drawables, camera_projection, frame_context.acquire_swap_chain_image_barrier,
            &pimpl_->tex_desc_set_cache, &pimpl_->geo_vram, &pimpl_->tex_vram);

        std::vector<LightSourceInstance> light_sources =
            ToLightSources(scene_entities, camera_projection);
        for (auto instance : light_sources) {
            switch (instance.light_source.model_case()) {
            case LightSource::ModelCase::kSunLight: {
                pimpl_->directional_radiance_pipeline.Run(instance.light_source.sun_light(),
                                                          *light_inputs);
                break;
            }
            case LightSource::ModelCase::kPointLight: {
                // TODO: Implements point light.
                break;
            }
            case LightSource::ModelCase::kSpotLight: {
                // TODO: Implements spot light.
                break;
            }
            default: {
                assert(false);
            }
            }
        }

        final_output = pimpl_->tone_map_pipeline.Run(pimpl_->radiance_output);
    }
    this->EndStage(/*index=*/1);

    this->EndFrame(frame_context, final_output);
}

void RadianceRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
