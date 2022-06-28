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
#include "renderer/output/common_output.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/postprocessor/exposure.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/postprocessor/tone_map.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

PipelineKey const kClampedLinearToneMapPipeline = "Clamped Linear Tone Map";
PipelineKey const kAcesToneMapPipeline = "ACES Tone Map";

class ToneMapPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    ToneMapPostProcessorConfigurator(PipelineStage const &radiance_map_stage,
                                     PipelineStage *exposure_stage);
    ~ToneMapPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    PipelineOutputInterface const &radiance_;
    PipelineOutputInterface const *exposure_;
};

ToneMapPostProcessorConfigurator::ToneMapPostProcessorConfigurator(
    PipelineStage const &radiance_map_stage, PipelineStage *exposure_stage)
    : radiance_(*radiance_map_stage.Output()) {
    if (exposure_stage != nullptr) {
        exposure_ = exposure_stage->Output();
    } else {
        exposure_ = nullptr;
    }
}

ToneMapPostProcessorConfigurator::~ToneMapPostProcessorConfigurator() {}

void ToneMapPostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = radiance_.ColorAttachments()[0]->view;

    if (exposure_ != nullptr) {
        input_images->at(1) = exposure_->ColorAttachments()[0]->view;
    }
}

} // namespace

std::unique_ptr<PipelineStage> CreateLdrImageStage(unsigned width, unsigned height,
                                                   VulkanContext *context) {
    auto output = std::make_shared<LdrColorPipelineOutput>(width, height,
                                                           /*with_depth_buffer=*/false, context);
    return std::make_unique<PipelineStage>(output);
}

void DoToneMapping(PipelineStage *radiance_map, PipelineStage *exposure,
                   DescriptorSetAllocator *desc_set_allocator, VulkanContext *context,
                   PipelineStage *target) {
    CachedPipelineInterface *pipeline;

    if (exposure != nullptr) {
        pipeline = target->WithPipeline(
            kAcesToneMapPipeline,
            [desc_set_allocator, context](PipelineOutputInterface *tone_map_output) {
                return std::make_unique<PostProcessorPipeline>(
                    kAcesToneMapPipeline, kFragmentShaderFilePathHdrAces, /*input_image_count=*/2,
                    /*push_constant_size=*/0, tone_map_output, desc_set_allocator, context);
            });
    } else {
        pipeline = target->WithPipeline(
            kClampedLinearToneMapPipeline,
            [desc_set_allocator, context](PipelineOutputInterface *tone_map_output) {
                return std::make_unique<PostProcessorPipeline>(
                    kClampedLinearToneMapPipeline, kFragmentShaderFilePathHdrClamp,
                    /*input_image_count=*/1, /*push_constant_size=*/0, tone_map_output,
                    desc_set_allocator, context);
            });
    }

    auto configurator = std::make_unique<ToneMapPostProcessorConfigurator>(*radiance_map, exposure);
    target->Schedule(pipeline, std::move(configurator),
                     /*parents=*/std::vector<PipelineStage *>{radiance_map, exposure});
}

} // namespace e8
