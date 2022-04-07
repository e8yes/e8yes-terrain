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
#include "renderer/output/pipeline_output.h"
#include "renderer/pass/render_pass.h"
#include "renderer/pipeline/pipeline_solid_color.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "renderer/renderer_solid_color.h"
#include "resource/accessor.h"

namespace e8 {

struct SolidColorRenderer::SolidColorRendererImpl {
    SolidColorRendererImpl(VulkanContext *context);
    ~SolidColorRendererImpl();

    SwapChainPipelineOutput output;
    SolidColorPipeline pipeline;
};

SolidColorRenderer::SolidColorRendererImpl::SolidColorRendererImpl(VulkanContext *context)
    : output(/*with_depth_buffer=*/false, context), pipeline(&output, context) {}

SolidColorRenderer::SolidColorRendererImpl::~SolidColorRendererImpl() {}

SolidColorRenderer::SolidColorRenderer(VulkanContext *context)
    : RendererInterface(0, /*max_frame_duration=*/std::chrono::seconds(10), context),
      pimpl_(std::make_unique<SolidColorRendererImpl>(context)) {}

SolidColorRenderer::~SolidColorRenderer() {}

void SolidColorRenderer::DrawFrame(Scene *scene, ResourceAccessor * /*resource_accessor*/) {
    FrameContext frame_context = this->BeginFrame();

    pimpl_->output.SetSwapChainImageIndex(frame_context.swap_chain_image_index);

    PipelineOutputInterface *final_output;
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();
        final_output = pimpl_->pipeline.Run(scene->background_color,
                                            frame_context.acquire_swap_chain_image_barrier);
    }

    this->EndFrame(frame_context, final_output);
}

void SolidColorRenderer::ApplyConfiguration(RendererConfiguration const & /*config*/) {}

} // namespace e8
