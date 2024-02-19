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
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/drawable/collection.h"
#include "renderer/lighting/direct_illuminator.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_radiance.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/space_screen/exposure.h"
#include "renderer/space_screen/fxaa.h"
#include "renderer/space_screen/tone_map.h"
#include "resource/accessor.h"

namespace e8 {

struct RadianceRenderer::RadianceRendererImpl {
    RadianceRendererImpl(VulkanContext *context);
    ~RadianceRendererImpl();

    DagContext dag_context;
    FrameResourceAllocator frame_resource_allocator;
    RendererConfiguration config;
};

RadianceRenderer::RadianceRendererImpl::RadianceRendererImpl(VulkanContext *context)
    : dag_context(context), frame_resource_allocator(context) {}

RadianceRenderer::RadianceRendererImpl::~RadianceRendererImpl() {}

RadianceRenderer::RadianceRenderer(VulkanContext *context)
    : RendererInterface(/*num_stages=*/1, context),
      pimpl_(std::make_unique<RadianceRendererImpl>(context)) {}

RadianceRenderer::~RadianceRenderer() {}

void RadianceRenderer::DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) {
    std::shared_ptr<SwapChainOutput> render_output =
        this->AcquireFinalColorImage(&pimpl_->frame_resource_allocator);

    Scene::ReadAccess read_access = scene->GainReadAccess();
    PerspectiveProjection projection(scene->camera);
    DrawableCollection drawable_collection(*scene->SceneEntityStructure(), resource_accessor);

    // Render passes.
    DagContext::Session session = pimpl_->dag_context.CreateSession();

    DagOperationInstance projected_surface =
        DoProjectSurface(&drawable_collection, projection, render_output->Width(),
                         render_output->Height(), &session);
    DagOperationInstance radiance_map =
        DoComputeDirectIllumination(&drawable_collection, projected_surface, projection, &session);

    //    DagOperationInstance log_exposure_value =
    //        DoEstimateExposure(radiance_map, &pimpl_->dag_context);
    DagOperationInstance ldr_image = DoToneMapping(radiance_map, nullptr, &session);
    DagOperationInstance final_result = DoFxaa(ldr_image, render_output, &session);
    std::vector<GpuPromise *> final_waits =
        final_result->Fulfill(/*wait=*/false, &pimpl_->frame_resource_allocator);

    this->PresentFinalColorImage(*render_output, final_waits);
}

void RadianceRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
