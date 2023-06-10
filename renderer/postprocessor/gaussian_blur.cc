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
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/common_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/postprocessor/gaussian_blur.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kHorizontalGaussianBlurPipelinePrefix = "Horizontal Gaussian Blur";
PipelineKey const kVerticalGaussianBlurPipelinePrefix = "Vertical Gaussian Blur";

class GaussianBlurPipelineConfigurator : public PostProcessorConfiguratorInterface {
  public:
    GaussianBlurPipelineConfigurator(PipelineOutputInterface const &input);
    ~GaussianBlurPipelineConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    PipelineOutputInterface const &input_;
};

GaussianBlurPipelineConfigurator::GaussianBlurPipelineConfigurator(
    PipelineOutputInterface const &input)
    : input_(input) {}

GaussianBlurPipelineConfigurator::~GaussianBlurPipelineConfigurator() {}

void GaussianBlurPipelineConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = input_.ColorAttachments()[0]->view;
}

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

std::unique_ptr<CachedPipelineInterface>
CreateGaussianBlurPipeline(bool horizontal, GaussianBlurLevel blur_level,
                           PipelineOutputInterface *blurred_output,
                           TransferContext *transfer_context) {
    PipelineKey pipeline_key;
    std::string shader;

    if (horizontal) {
        pipeline_key = HorizontalGaussianBlurPipelineKey(blur_level);
        shader = HorizontalGaussianBlurShader(blur_level);
    } else {
        pipeline_key = VerticalGaussianBlurPipelineKey(blur_level);
        shader = VerticalGaussianBlurShader(blur_level);
    }

    return std::make_unique<PostProcessorPipeline>(pipeline_key, shader, /*input_image_count=*/1,
                                                   /*push_constant_size=*/0, blurred_output,
                                                   transfer_context);
}

} // namespace

void CreateGaussianBlurStages(unsigned width, unsigned height, VulkanContext *context,
                              std::unique_ptr<PipelineStage> *h_blurred,
                              std::unique_ptr<PipelineStage> *hv_blurred) {
    auto h_blurred_output = std::make_shared<FloatPipelineOutput>(width, height, context);
    auto hv_blurred_output = std::make_shared<FloatPipelineOutput>(width, height, context);

    *h_blurred = std::make_unique<PipelineStage>(h_blurred_output);
    *hv_blurred = std::make_unique<PipelineStage>(hv_blurred_output);
}

void DoGaussianBlur(PipelineStage *image, GaussianBlurLevel blur_level,
                    TransferContext *transfer_context, PipelineStage *h_blurred,
                    PipelineStage *hv_blurred) {
    CachedPipelineInterface *h_blur_pipeline = h_blurred->WithPipeline(
        HorizontalGaussianBlurPipelineKey(blur_level),
        [blur_level, transfer_context](PipelineOutputInterface *blurred_output) {
            return CreateGaussianBlurPipeline(/*horizontal=*/true, blur_level, blurred_output,
                                              transfer_context);
        });
    CachedPipelineInterface *v_blur_pipeline = hv_blurred->WithPipeline(
        VerticalGaussianBlurPipelineKey(blur_level),
        [blur_level, transfer_context](PipelineOutputInterface *blurred_output) {
            return CreateGaussianBlurPipeline(/*horizontal=*/false, blur_level, blurred_output,
                                              transfer_context);
        });

    auto h_blur_configurator = std::make_unique<GaussianBlurPipelineConfigurator>(*image->Output());
    h_blurred->Schedule(h_blur_pipeline, std::move(h_blur_configurator),
                        /*parents=*/std::vector<PipelineStage *>{image});

    auto v_blur_configurator =
        std::make_unique<GaussianBlurPipelineConfigurator>(*h_blurred->Output());
    hv_blurred->Schedule(v_blur_pipeline, std::move(v_blur_configurator),
                         /*parents=*/std::vector<PipelineStage *>{h_blurred});
}

} // namespace e8
