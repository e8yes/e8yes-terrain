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

#include <memory>

#include "common/device.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/postprocessor/fxaa.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

PipelineKey const &kFxaaPipeline = "FXAA";

class FxaaPipelineConfigurator : public PostProcessorConfiguratorInterface {
  public:
    FxaaPipelineConfigurator(PipelineOutputInterface const &ldr_color_input);
    ~FxaaPipelineConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    PipelineOutputInterface const &ldr_color_input_;
};

FxaaPipelineConfigurator::FxaaPipelineConfigurator(PipelineOutputInterface const &ldr_color_input)
    : ldr_color_input_(ldr_color_input) {}

FxaaPipelineConfigurator::~FxaaPipelineConfigurator() {}

void FxaaPipelineConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = ldr_color_input_.ColorAttachments()[0]->view;
}

} // namespace

void DoFxaa(PipelineStage *ldr_image, DescriptorSetAllocator *desc_set_allocator,
            VulkanContext *context, PipelineStage *target) {
    CachedPipelineInterface *pipeline = target->WithPipeline(
        kFxaaPipeline, [desc_set_allocator, context](PipelineOutputInterface *aa_output) {
            return std::make_unique<PostProcessorPipeline>(
                kFxaaPipeline, kFragmentShaderFilePathFxaa, /*input_image_count=*/1,
                /*push_constant_size=*/0, aa_output, desc_set_allocator, context);
        });

    auto configurator = std::make_unique<FxaaPipelineConfigurator>(*ldr_image->Output());
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<PipelineStage *>{ldr_image});
}

} // namespace e8
