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
    ToneMapPostProcessorConfigurator(UnboundedColorPipelineOutput const &radiance);
    ~ToneMapPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    UnboundedColorPipelineOutput const &radiance_;
};

ToneMapPostProcessorConfigurator::ToneMapPostProcessorConfigurator(
    UnboundedColorPipelineOutput const &radiance)
    : radiance_(radiance) {}

ToneMapPostProcessorConfigurator::~ToneMapPostProcessorConfigurator() {}

void ToneMapPostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = radiance_.ColorAttachments()[0]->view;
}

void ToneMapPostProcessorConfigurator::PushConstants(
    std::vector<uint8_t> * /*push_constants*/) const {}

} // namespace

struct ToneMapPipeline::ToneMapPipelineImpl {
    ToneMapPipelineImpl(PipelineOutputInterface *color_output,
                        DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~ToneMapPipelineImpl();

    VulkanContext *context;
    DescriptorSetAllocator *desc_set_allocator;
    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;
};

ToneMapPipeline::ToneMapPipelineImpl::ToneMapPipelineImpl(
    PipelineOutputInterface *color_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context)
    : context(context) {
    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathHdrClamp, /*input_image_count=*/1, /*push_constant_size=*/0,
        color_output, desc_set_allocator, context);
}

ToneMapPipeline::ToneMapPipelineImpl::~ToneMapPipelineImpl() {}

ToneMapPipeline::ToneMapPipeline(PipelineOutputInterface *color_output,
                                 DescriptorSetAllocator *desc_set_allocator, VulkanContext *context)
    : pimpl_(std::make_unique<ToneMapPipelineImpl>(color_output, desc_set_allocator, context)) {}

ToneMapPipeline::~ToneMapPipeline() {}

PipelineOutputInterface *ToneMapPipeline::Run(UnboundedColorPipelineOutput const &radiance) {
    ToneMapPostProcessorConfigurator configurator(radiance);
    return pimpl_->post_processor_pipeline->Run(configurator, *radiance.Promise());
}

} // namespace e8
