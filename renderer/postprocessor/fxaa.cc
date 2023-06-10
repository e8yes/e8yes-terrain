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
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/postprocessor/fxaa.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

PipelineKey const &kFxaaPipeline = "FXAA";

class FxaaPipelineConfigurator : public PostProcessorConfiguratorInterface {
   public:
    FxaaPipelineConfigurator(GraphicsPipelineOutputInterface const &ldr_color_input);
    ~FxaaPipelineConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

   private:
    GraphicsPipelineOutputInterface const &ldr_color_input_;
};

FxaaPipelineConfigurator::FxaaPipelineConfigurator(GraphicsPipelineOutputInterface const &ldr_color_input)
    : ldr_color_input_(ldr_color_input) {}

FxaaPipelineConfigurator::~FxaaPipelineConfigurator() {}

void FxaaPipelineConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = ldr_color_input_.ColorAttachments()[0]->view;
}

}  // namespace

void DoFxaa(DagOperation *ldr_image, TransferContext *transfer_context, DagOperation *target) {
    GraphicsPipelineInterface *pipeline =
        target->WithPipeline(kFxaaPipeline, [transfer_context](GraphicsPipelineOutputInterface *aa_output) {
            return std::make_unique<PostProcessorPipeline>(
                kFxaaPipeline, kFragmentShaderFilePathFxaa, /*input_image_count=*/1,
                /*push_constant_size=*/0, aa_output, transfer_context);
        });

    auto configurator = std::make_unique<FxaaPipelineConfigurator>(*ldr_image->Output());
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<DagOperation *>{ldr_image});
}

}  // namespace e8
