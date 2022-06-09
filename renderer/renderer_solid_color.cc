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
#include "renderer/output/pipeline_stage.h"
#include "renderer/pass/rasterize.h"
#include "renderer/pipeline/solid_color.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_solid_color.h"
#include "resource/accessor.h"

namespace e8 {

struct SolidColorRenderer::SolidColorRendererImpl {
    SolidColorRendererImpl(std::shared_ptr<PipelineOutputInterface> const &final_output);
    ~SolidColorRendererImpl();

    PipelineStage fill_color;
};

SolidColorRenderer::SolidColorRendererImpl::SolidColorRendererImpl(
    std::shared_ptr<PipelineOutputInterface> const &final_output)
    : fill_color(final_output) {}

SolidColorRenderer::SolidColorRendererImpl::~SolidColorRendererImpl() {}

SolidColorRenderer::SolidColorRenderer(VulkanContext *context)
    : RendererInterface(0, context),
      pimpl_(std::make_unique<SolidColorRendererImpl>(RendererInterface::FinalOutput())) {}

SolidColorRenderer::~SolidColorRenderer() {}

void SolidColorRenderer::DrawFrame(Scene *scene, ResourceAccessor * /*resource_accessor*/) {
    Scene::ReadAccess read_access = scene->GainReadAccess();

    PipelineStage *first_stage = this->DoFirstStage();
    DoFillColor(scene->background_color, /*parents=*/std::vector<PipelineStage *>{first_stage},
                context, &pimpl_->fill_color);
    PipelineStage *final_stage = this->DoFinalStage(
        /*parents=*/std::vector<PipelineStage *>{first_stage, &pimpl_->fill_color});

    final_stage->Fulfill(context);
}

void SolidColorRenderer::ApplyConfiguration(RendererConfiguration const & /*config*/) {}

} // namespace e8
