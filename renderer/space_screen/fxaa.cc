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

#include "renderer/basic/shader.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/space_screen/fxaa.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const &kFxaaPipeline = "FXAA";

class FxaaPipelineConfigurator : public ScreenSpaceConfiguratorInterface {
  public:
    FxaaPipelineConfigurator(GraphicsPipelineOutputInterface const &ldr_color_input);
    ~FxaaPipelineConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    GraphicsPipelineOutputInterface const &ldr_color_input_;
};

FxaaPipelineConfigurator::FxaaPipelineConfigurator(
    GraphicsPipelineOutputInterface const &ldr_color_input)
    : ldr_color_input_(ldr_color_input) {}

FxaaPipelineConfigurator::~FxaaPipelineConfigurator() {}

void FxaaPipelineConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = ldr_color_input_.ColorAttachments()[0]->view;
}

} // namespace

DagOperationInstance
DoFxaa(DagOperationInstance ldr_image,
       const std::shared_ptr<GraphicsPipelineOutputInterface> &color_image_output,
       DagContext::Session *session) {
    DagOperationInstance target = session->WithOperation(
        kFxaaPipeline, ldr_image->Output()->Width(), ldr_image->Output()->Height(),
        [color_image_output](unsigned /*width*/, unsigned /*height*/,
                             TransferContext *transfer_context, VulkanContext *vulkan_context) {
            return std::make_unique<DagOperation>(color_image_output, transfer_context,
                                                  vulkan_context);
        });

    GraphicsPipelineInterface *pipeline = target->WithPipeline(
        kFxaaPipeline, [](GraphicsPipelineOutputInterface *aa_output,
                          TransferContext *transfer_context, VulkanContext *vulkan_context) {
            return std::make_unique<ScreenSpaceProcessorPipeline>(
                kFxaaPipeline, kFragmentShaderFilePathFxaa, /*input_image_count=*/1,
                /*push_constant_size=*/0, aa_output, transfer_context, vulkan_context);
        });

    auto configurator = std::make_unique<FxaaPipelineConfigurator>(*ldr_image->Output());
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<DagOperation *>{ldr_image});

    return target;
}

} // namespace e8
