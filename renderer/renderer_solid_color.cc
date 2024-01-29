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
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "content/scene.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
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
};

SolidColorRenderer::SolidColorRendererImpl::SolidColorRendererImpl(VulkanContext *context)
    : dag_context(context) {}

SolidColorRenderer::SolidColorRendererImpl::~SolidColorRendererImpl() {}

SolidColorRenderer::SolidColorRenderer(VulkanContext *context)
    : RendererInterface(0, context), pimpl_(std::make_unique<SolidColorRendererImpl>(context)) {}

SolidColorRenderer::~SolidColorRenderer() {}

void SolidColorRenderer::DrawFrame(Scene *scene, ResourceAccessor * /*resource_accessor*/) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    DagContext::Session session = pimpl_->dag_context.CreateSession();

    DagOperation *first_stage = this->DoFirstStage();
    std::shared_ptr<GraphicsPipelineOutputInterface> final_color_output =
        RendererInterface::FinalColorImage();
    DagOperationInstance filled_image = DoFillColor(
        scene->background_color, first_stage, final_color_output, context, &pimpl_->dag_context);
    DagOperation *final_stage = this->DoFinalStage(first_stage, filled_image);

    final_stage->Fulfill(context);
}

void SolidColorRenderer::ApplyConfiguration(RendererConfiguration const & /*config*/) {}

} // namespace e8
