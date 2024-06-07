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
#include <memory>
#include <string>

#include "common/device.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
#include "renderer/space_screen/gaussian_blur.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kHorizontalGaussianBlurPipelinePrefix = "Horizontal Gaussian Blur";
PipelineKey const kVerticalGaussianBlurPipelinePrefix = "Vertical Gaussian Blur";

class GaussianBlurPipelineUniforms : public ScreenSpaceUniformsInterface {
  public:
    GaussianBlurPipelineUniforms(GraphicsPipelineOutputInterface const *input) : input_(input) {}
    ~GaussianBlurPipelineUniforms() override = default;

    void InputImages(std::vector<VkImageView> *input_images) const override {
        input_images->at(0) = input_->ColorAttachments()[0]->view;
    }

  private:
    GraphicsPipelineOutputInterface const *input_;
};

PipelineKey HorizontalGaussianBlurPipelineKey(GaussianBlurLevel blur_level) {
    switch (blur_level) {
    case GBL_1X1: {
        return kHorizontalGaussianBlurPipelinePrefix + " 1x1";
    }
    case GBL_3X3: {
        return kHorizontalGaussianBlurPipelinePrefix + " 3x3";
    }
    case GBL_5X5: {
        return kHorizontalGaussianBlurPipelinePrefix + " 5x5";
    }
    default: {
        assert(false);
    }
    }
}

PipelineKey VerticalGaussianBlurPipelineKey(GaussianBlurLevel blur_level) {
    switch (blur_level) {
    case GBL_1X1: {
        return kVerticalGaussianBlurPipelinePrefix + " 1x1";
    }
    case GBL_3X3: {
        return kVerticalGaussianBlurPipelinePrefix + " 3x3";
    }
    case GBL_5X5: {
        return kVerticalGaussianBlurPipelinePrefix + " 5x5";
    }
    default: {
        assert(false);
    }
    }
}

std::string HorizontalGaussianBlurShader(GaussianBlurLevel blur_level) {
    switch (blur_level) {
    case GBL_1X1: {
        return kFragmentShaderFilePathBlurGaussianHorizontal1X1;
    }
    case GBL_3X3: {
        return kFragmentShaderFilePathBlurGaussianHorizontal3X3;
    }
    case GBL_5X5: {
        return kFragmentShaderFilePathBlurGaussianHorizontal5X5;
    }
    default: {
        assert(false);
    }
    }
}

std::string VerticalGaussianBlurShader(GaussianBlurLevel blur_level) {
    switch (blur_level) {
    case GBL_1X1: {
        return kFragmentShaderFilePathBlurGaussianVertical1X1;
    }
    case GBL_3X3: {
        return kFragmentShaderFilePathBlurGaussianVertical3X3;
    }
    case GBL_5X5: {
        return kFragmentShaderFilePathBlurGaussianVertical5X5;
    }
    default: {
        assert(false);
    }
    }
}

std::unique_ptr<GraphicsPipelineInterface>
CreateGaussianBlurPipeline(bool horizontal, GaussianBlurLevel blur_level,
                           GraphicsPipelineOutputInterface *blurred_output,
                           VulkanContext *vulkan_context) {
    PipelineKey pipeline_key;
    std::string shader;

    if (horizontal) {
        pipeline_key = HorizontalGaussianBlurPipelineKey(blur_level);
        shader = HorizontalGaussianBlurShader(blur_level);
    } else {
        pipeline_key = VerticalGaussianBlurPipelineKey(blur_level);
        shader = VerticalGaussianBlurShader(blur_level);
    }

    return std::make_unique<ScreenSpaceProcessorPipeline>(
        pipeline_key, shader, /*input_image_count=*/1, /*push_constant_size=*/0, blurred_output,
        vulkan_context);
}

} // namespace

std::unique_ptr<DagOperation> CreateGaussianBlurOp(unsigned width, unsigned height,
                                                   TransferContext *transfer_context,
                                                   VulkanContext *vulkan_context) {
    auto blurred_output =
        std::make_shared<FloatOutput>(width, height, /*with_depth_buffer=*/false, vulkan_context);
    return std::make_unique<DagOperation>(blurred_output, transfer_context, vulkan_context);
}

DagOperationInstance DoGaussianBlur(DagOperationInstance image, GaussianBlurLevel blur_level,
                                    DagContext::Session *session) {
    PipelineKey horizontal_blur_pipeline_key = HorizontalGaussianBlurPipelineKey(blur_level);
    PipelineKey veritical_blur_pipeline_key = VerticalGaussianBlurPipelineKey(blur_level);

    DagOperationInstance h_blurred =
        session->WithOperation(horizontal_blur_pipeline_key, image->Output()->Width(),
                               image->Output()->Height(), CreateGaussianBlurOp);
    DagOperationInstance hv_blurred =
        session->WithOperation(veritical_blur_pipeline_key, image->Output()->Width(),
                               image->Output()->Height(), CreateGaussianBlurOp);

    GraphicsPipelineInterface *h_blur_pipeline = h_blurred->WithPipeline(
        HorizontalGaussianBlurPipelineKey(blur_level),
        [blur_level](GraphicsPipelineOutputInterface *blurred_output,
                     VulkanContext *vulkan_context) {
            return CreateGaussianBlurPipeline(/*horizontal=*/true, blur_level, blurred_output,
                                              vulkan_context);
        });
    GraphicsPipelineInterface *v_blur_pipeline = hv_blurred->WithPipeline(
        VerticalGaussianBlurPipelineKey(blur_level),
        [blur_level](GraphicsPipelineOutputInterface *blurred_output,
                     VulkanContext *vulkan_context) {
            return CreateGaussianBlurPipeline(/*horizontal=*/false, blur_level, blurred_output,
                                              vulkan_context);
        });

    auto h_blur_configurator = std::make_unique<GaussianBlurPipelineUniforms>(image->Output());
    h_blurred->Schedule(h_blur_pipeline, std::move(h_blur_configurator),
                        /*parents=*/std::vector<DagOperationInstance>{image});
    auto v_blur_configurator = std::make_unique<GaussianBlurPipelineUniforms>(h_blurred->Output());
    hv_blurred->Schedule(v_blur_pipeline, std::move(v_blur_configurator),
                         /*parents=*/std::vector<DagOperationInstance>{h_blurred});

    return hv_blurred;
}

} // namespace e8
