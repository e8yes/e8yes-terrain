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
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/postprocessor/depth_map_visualizer.h"
#include "renderer/postprocessor/post_processor.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

struct DepthMapVisualizerParameters {
    float z_near;
    float z_far;
    float alpha;
};

class DepthMapPostProcessorConfigurator : public PostProcessorConfiguratorInterface {
  public:
    DepthMapPostProcessorConfigurator(float alpha, std::optional<PerspectiveProjection> projection,
                                      DepthMapPipelineOutput const &depth_map);
    ~DepthMapPostProcessorConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;
    void PushConstants(std::vector<uint8_t> *push_constants) const override;

  private:
    float const alpha_;
    std::optional<PerspectiveProjection> const projection_;
    DepthMapPipelineOutput const &depth_map_;
};

DepthMapPostProcessorConfigurator::DepthMapPostProcessorConfigurator(
    float alpha, std::optional<PerspectiveProjection> projection,
    DepthMapPipelineOutput const &depth_map)
    : alpha_(alpha), projection_(projection), depth_map_(depth_map) {}

DepthMapPostProcessorConfigurator::~DepthMapPostProcessorConfigurator() {}

void DepthMapPostProcessorConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = depth_map_.DepthAttachment()->view;
}

void DepthMapPostProcessorConfigurator::PushConstants(std::vector<uint8_t> *push_constants) const {
    DepthMapVisualizerParameters *parameters =
        reinterpret_cast<DepthMapVisualizerParameters *>(push_constants->data());

    parameters->alpha = alpha_;

    if (projection_.has_value()) {
        parameters->z_near = projection_->Frustum().z_near;
        parameters->z_far = projection_->Frustum().z_far;
    } else {
        parameters->z_near = 1.0f;
        parameters->z_far = 100.0f;
    }
}

} // namespace

struct DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl {
    DepthMapVisualizerPipelineImpl(PipelineOutputInterface *visualizer_output,
                                   DescriptorSetAllocator *desc_set_allocator,
                                   VulkanContext *context);
    ~DepthMapVisualizerPipelineImpl();

    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;
};

DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl::DepthMapVisualizerPipelineImpl(
    PipelineOutputInterface *visualizer_output, DescriptorSetAllocator *desc_set_allocator,
    VulkanContext *context) {
    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathDepthMapVisualizer, /*input_image_count=*/1,
        /*push_constant_size=*/sizeof(DepthMapVisualizerParameters), visualizer_output,
        desc_set_allocator, context);
}

DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl::~DepthMapVisualizerPipelineImpl() {}

DepthMapVisualizerPipeline::DepthMapVisualizerPipeline(DescriptorSetAllocator *desc_set_allocator,
                                                       VulkanContext *context)
    : desc_set_allocator_(desc_set_allocator), context_(context), current_output_(nullptr) {}

DepthMapVisualizerPipeline::~DepthMapVisualizerPipeline() {}

void DepthMapVisualizerPipeline::Run(float alpha, std::optional<PerspectiveProjection> projection,
                                     DepthMapPipelineOutput const &depth_map,
                                     PipelineOutputInterface *visualizer_output) {
    if (visualizer_output != current_output_) {
        pimpl_ = std::make_unique<DepthMapVisualizerPipelineImpl>(visualizer_output,
                                                                  desc_set_allocator_, context_);
        current_output_ = visualizer_output;
    }

    DepthMapPostProcessorConfigurator configurator(alpha, projection, depth_map);
    pimpl_->post_processor_pipeline->Run(configurator, *depth_map.Promise());
}

} // namespace e8
