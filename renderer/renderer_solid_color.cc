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

#include <chrono>
#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "content/scene.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_solid_color.h"
#include "renderer/space_screen/fill_color.h"
#include "resource/accessor.h"

namespace e8 {

struct SolidColorRenderer::SolidColorRendererImpl {
    SolidColorRendererImpl(VulkanContext *context);
    ~SolidColorRendererImpl();

    DagContext dag_context;
    FrameResourceAllocator frame_resource_allocator;
};

SolidColorRenderer::SolidColorRendererImpl::SolidColorRendererImpl(VulkanContext *context)
    : dag_context(context), frame_resource_allocator(context) {}

SolidColorRenderer::SolidColorRendererImpl::~SolidColorRendererImpl() {}

SolidColorRenderer::SolidColorRenderer(VulkanContext *context)
    : RendererInterface(0, context), pimpl_(std::make_unique<SolidColorRendererImpl>(context)) {}

SolidColorRenderer::~SolidColorRenderer() {}

void SolidColorRenderer::DrawFrame(Scene *scene, ResourceAccessor * /*resource_accessor*/) {
    std::shared_ptr<SwapChainOutput> render_output =
        this->AcquireFinalColorImage(&pimpl_->frame_resource_allocator);

    Scene::ReadAccess read_access = scene->GainReadAccess();

    DagContext::Session session = pimpl_->dag_context.CreateSession();
    DagOperationInstance filled_image =
        DoFillColor(scene->background_color, render_output, &session);
    std::vector<GpuPromise *> final_waits =
        filled_image->Fulfill(/*wait=*/false, &pimpl_->frame_resource_allocator);
    this->PresentFinalColorImage(*render_output, final_waits);
}

void SolidColorRenderer::ApplyConfiguration(RendererConfiguration const & /*config*/) {}

} // namespace e8
