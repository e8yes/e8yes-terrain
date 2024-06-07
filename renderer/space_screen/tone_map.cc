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
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/shader.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/space_screen/tone_map.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kClampedLinearToneMapPipeline = "Clamped Linear Tone Map";
PipelineKey const kAcesToneMapPipeline = "ACES Tone Map";

class ToneMapPostProcessorUniforms : public ScreenSpaceUniformsInterface {
  public:
    ToneMapPostProcessorUniforms(DagOperationInstance radiance_map, DagOperationInstance exposure)
        : radiance_(radiance_map->Output()) {
        if (exposure != nullptr) {
            exposure_ = exposure->Output();
        } else {
            exposure_ = nullptr;
        }
    }

    ~ToneMapPostProcessorUniforms() override = default;

    void InputImages(std::vector<VkImageView> *input_images) const override {
        input_images->at(0) = radiance_->ColorAttachments()[0]->view;

        if (exposure_ != nullptr) {
            input_images->at(1) = exposure_->ColorAttachments()[0]->view;
        }
    }

  private:
    GraphicsPipelineOutputInterface const *radiance_;
    GraphicsPipelineOutputInterface const *exposure_;
};

/**
 * @brief CreateLdrImageStage Creates an LDR (low dynamic range) image stage with a 32-bit RGBA
 * color image output in the specified dimension.
 *
 * @param width The width of the color image.
 * @param height The height of the color image.
 * @param context Contextual Vulkan handles.
 * @return An LDR image stage with a color image output.
 */
std::unique_ptr<DagOperation> CreateToneMappingOp(unsigned width, unsigned height,
                                                  TransferContext *transfer_context,
                                                  VulkanContext *vulkan_context) {
    auto output = std::make_shared<LdrColorOutput>(width, height,
                                                   /*with_depth_buffer=*/false, vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
}

} // namespace

DagOperationInstance DoToneMapping(DagOperationInstance radiance_map, DagOperationInstance exposure,
                                   DagContext::Session *session) {
    PipelineKey pipeline_key =
        exposure != nullptr ? kAcesToneMapPipeline : kClampedLinearToneMapPipeline;
    DagOperationInstance target =
        session->WithOperation(pipeline_key, radiance_map->Output()->Width(),
                               radiance_map->Output()->Height(), CreateToneMappingOp);

    GraphicsPipelineInterface *pipeline;
    if (exposure != nullptr) {
        pipeline = target->WithPipeline(
            kAcesToneMapPipeline,
            [](GraphicsPipelineOutputInterface *tone_map_output, VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kAcesToneMapPipeline, kFragmentShaderFilePathHdrAces, /*input_image_count=*/2,
                    /*push_constant_size=*/0, tone_map_output, vulkan_context);
            });
    } else {
        pipeline = target->WithPipeline(
            kClampedLinearToneMapPipeline,
            [](GraphicsPipelineOutputInterface *tone_map_output, VulkanContext *vulkan_context) {
                return std::make_unique<ScreenSpaceProcessorPipeline>(
                    kClampedLinearToneMapPipeline, kFragmentShaderFilePathHdrClamp,
                    /*input_image_count=*/1, /*push_constant_size=*/0, tone_map_output,
                    vulkan_context);
            });
    }

    auto configurator = std::make_unique<ToneMapPostProcessorUniforms>(radiance_map, exposure);
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<DagOperationInstance>{radiance_map, exposure});

    return target;
}

} // namespace e8
