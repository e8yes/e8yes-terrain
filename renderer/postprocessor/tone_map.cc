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
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/postprocessor/tone_map.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

class ToneMapPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    ToneMapPostProcessorConfigurator(UnboundedColorPipelineOutput const &radiance,
                                     ExposureEstimationPipelineOutput const *exposure);
    ~ToneMapPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    UnboundedColorPipelineOutput const &radiance_;
    ExposureEstimationPipelineOutput const *exposure_;
};

ToneMapPostProcessorConfigurator::ToneMapPostProcessorConfigurator(
    UnboundedColorPipelineOutput const &radiance, ExposureEstimationPipelineOutput const *exposure)
    : radiance_(radiance), exposure_(exposure) {}

ToneMapPostProcessorConfigurator::~ToneMapPostProcessorConfigurator() {}

void ToneMapPostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = radiance_.ColorAttachments()[0]->view;

    if (exposure_ != nullptr) {
        input_images->at(1) = exposure_->ColorAttachments()[0]->view;
    }
}

void ToneMapPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> * /*push_constants*/) const {}

} // namespace

struct ToneMapPipeline::ToneMapPipelineImpl {
    ToneMapPipelineImpl(PipelineOutputInterface *color_output,
                        DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~ToneMapPipelineImpl();

    std::unique_ptr<PostProcessorPipeline> hdr_clamp_pipeline;
    std::unique_ptr<PostProcessorPipeline> hdr_aces_pipeline;
};

ToneMapPipeline::ToneMapPipelineImpl::ToneMapPipelineImpl(
    PipelineOutputInterface *color_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context) {
    hdr_clamp_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathHdrClamp, /*input_image_count=*/1, /*push_constant_size=*/0,
        color_output, desc_set_allocator, context);
    hdr_aces_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathHdrAces, /*input_image_count=*/2, /*push_constant_size=*/0,
        color_output, desc_set_allocator, context);
}

ToneMapPipeline::ToneMapPipelineImpl::~ToneMapPipelineImpl() {}

ToneMapPipeline::ToneMapPipeline(DescriptorSetAllocator *desc_set_allocator, VulkanContext *context)
    : desc_set_allocator_(desc_set_allocator), context_(context), current_output_(nullptr) {}

ToneMapPipeline::~ToneMapPipeline() {}

void ToneMapPipeline::Run(UnboundedColorPipelineOutput const &radiance,
                          ExposureEstimationPipelineOutput const *exposure,
                          PipelineOutputInterface *output) {
    if (output != current_output_) {
        pimpl_ = std::make_unique<ToneMapPipelineImpl>(output, desc_set_allocator_, context_);
        current_output_ = output;
    }

    ToneMapPostProcessorConfigurator configurator(radiance, exposure);
    if (exposure != nullptr) {
        pimpl_->hdr_aces_pipeline->Run(configurator, *exposure->Promise());
    } else {
        pimpl_->hdr_clamp_pipeline->Run(configurator, *radiance.Promise());
    }
}

} // namespace e8
