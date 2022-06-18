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

#ifndef ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H
#define ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The PostProcessorConfiguratorInterface class For configuring what shader uniform setup to
 * apply to the post processing pipeline.
 */
class PostProcessorConfiguratorInterface {
  public:
    PostProcessorConfiguratorInterface();
    virtual ~PostProcessorConfiguratorInterface();

    /**
     * @brief InputImages Returns the input images to the post processor. The input_images array has
     * been pre-allocated. Indexes the image by its descriptor binding.
     */
    virtual void InputImages(std::vector<VkImageView> *input_images) const;

    /**
     * @brief PushConstants A byte array containing push constant data of the post processing
     * pipeline.
     */
    virtual void PushConstants(std::vector<uint8_t> *push_constants) const;
};

/**
 * @brief The PostProcessorPipeline2 class A generic configurable post processing graphics pipeline.
 */
class PostProcessorPipeline2 : public CachedPipelineInterface {
  public:
    /**
     * @brief PostProcessorPipeline Constructs a custom post processor.
     *
     * @param fragment_shader The fragment shader to create the desired post processing effect.
     * @param input_image_count The number of input images the post processor requires.
     * @param push_constant_size The number of bytes the post processor's push constants requires.
     * @param output To receive output from the post processor.
     * @param desc_set_allocator Descriptor set allocator.
     * @param context Contextual Vulkan handles.
     */
    PostProcessorPipeline2(std::string const &fragment_shader, unsigned input_image_count,
                          unsigned push_constant_size, PipelineOutputInterface *output,
                          DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~PostProcessorPipeline2() override;

    Fulfillment Launch(unsigned instance_id,
                                   std::vector<PipelineOutputInterface *> const &inputs,
                                   std::vector<GpuPromise *> const &prerequisites,
                                   unsigned completion_signal_count,
                                   PipelineOutputInterface *output) override;

    /**
     * @brief SetConfigurator For configuring what shader uniform setup to apply to the post
     * processing pipeline.
     */
    void SetConfigurator(std::unique_ptr<PostProcessorConfiguratorInterface> &&configurator);

  private:
    struct PostProcessorPipelineImpl;
    std::unique_ptr<PostProcessorPipelineImpl> pimpl_;
};

/**
 * @brief The PostProcessorPipeline class It offers a basic framework for setting up a post
 * processing graphics pipeline. Supplies proper arguments to the constructor of this class to
 * define the actual effect.
 */
class PostProcessorPipeline {
  public:
    /**
     * @brief PostProcessorPipeline Constructs a custom post processor.
     *
     * @param fragment_shader The fragment shader to create the desired post processing effect.
     * @param input_image_count The number of input images the post processor requires.
     * @param push_constant_size The number of bytes the post processor's push constants requires.
     * @param output To receive output from the post processor.
     * @param desc_set_allocator Descriptor set allocator.
     * @param context Contextual Vulkan handles.
     */
    PostProcessorPipeline(std::string const &fragment_shader, unsigned input_image_count,
                          unsigned push_constant_size, PipelineOutputInterface *output,
                          DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);

    /**
     * @brief PostProcessorPipeline Constructs an empty post processor. It loads the empty post
     * processor shader and outputs the UV coordinate as color.
     *
     * @param output To receive output from the post processor.
     * @param desc_set_allocator Descriptor set allocator.
     * @param context Contextual Vulkan handles.
     */
    PostProcessorPipeline(PipelineOutputInterface *output,
                          DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);

    ~PostProcessorPipeline();

    /**
     * @brief Run Runs the post processing graphics pipeline. The pipeline can only be run when the
     * previous run was finished (indicated by the output's barrier).
     *
     * @param configurator For configuring what shader uniform setup to apply to the post processing
     * pipeline.
     * @param promise The previous tasks' promise.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(PostProcessorConfiguratorInterface const &configurator,
                                 GpuPromise const &promise);

  private:
    struct PostProcessorPipelineImpl;
    std::unique_ptr<PostProcessorPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H
