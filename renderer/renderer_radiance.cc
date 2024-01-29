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
#include "renderer/drawable/collection.h"
#include "renderer/lighting/direct_illuminator.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_radiance.h"
#include "renderer/space_projection/project_surface.h"
#include "renderer/space_screen/exposure.h"
#include "renderer/space_screen/fxaa.h"
#include "renderer/space_screen/tone_map.h"
#include "renderer/transfer/context.h"
#include "resource/accessor.h"

namespace e8 {

struct RadianceRenderer::RadianceRendererImpl {
    RadianceRendererImpl(std::unique_ptr<DagOperation> &&final_color_image, VulkanContext *context);
    ~RadianceRendererImpl();

    DagContext dag_context;
    TransferContext transfer_context;

    std::unique_ptr<DagOperation> log_luminance_map;
    std::unique_ptr<DagOperation> log_exposure_value;
    std::unique_ptr<DagOperation> ldr_image;
    std::unique_ptr<DagOperation> final_color_image;

    RendererConfiguration config;
};

RadianceRenderer::RadianceRendererImpl::RadianceRendererImpl(
    std::unique_ptr<DagOperation> &&final_color_image, VulkanContext *context)
    : dag_context(context), transfer_context(context),
      log_luminance_map(CreateLogLuminaneStage(context->swap_chain_image_extent.width,
                                               context->swap_chain_image_extent.height, context)),
      log_exposure_value(CreateExposureStage(context)),
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
    PerspectiveProjection projection(scene->camera);
    DrawableCollection drawable_collection(*scene->SceneEntityStructure(), resource_accessor);

    // Render passes.
    DagContext::Session session = pimpl_->dag_context.CreateSession();

    DagOperation *first_stage = this->DoFirstStage();
    DagOperationInstance projected_surface =
        DoProjectSurface(&drawable_collection, projection, first_stage, &pimpl_->transfer_context,
                         &pimpl_->dag_context);
    DagOperationInstance radiance_map =
        DoComputeDirectIllumination(&drawable_collection, projected_surface, projection,
                                    first_stage, &pimpl_->transfer_context, &pimpl_->dag_context);

    DoEstimateExposure(radiance_map, &pimpl_->transfer_context, pimpl_->log_luminance_map.get(),
                       pimpl_->log_exposure_value.get());
    DoToneMapping(radiance_map, pimpl_->log_exposure_value.get(), &pimpl_->transfer_context,
                  pimpl_->ldr_image.get());
    DoFxaa(pimpl_->ldr_image.get(), &pimpl_->transfer_context, pimpl_->final_color_image.get());

    DagOperation *final_stage =
        this->DoFinalStage(first_stage, pimpl_->final_color_image.get(),
                           /*dangling_stages=*/
                           std::vector<DagOperationInstance>{projected_surface});

    this->BeginStage(1, "FULFILL");
    final_stage->Fulfill(pimpl_->transfer_context.vulkan_context);
    this->EndStage(1);
}

void RadianceRenderer::ApplyConfiguration(RendererConfiguration const &config) {
    pimpl_->config = config;
}

} // namespace e8
