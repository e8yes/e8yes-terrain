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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_SCREEN_SPACE_PROCESSOR_H
#define ISLANDS_RENDERER_SPACE_SCREEN_SCREEN_SPACE_PROCESSOR_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief The ScreenSpaceConfiguratorInterface class For configuring what shader uniform setup to
 * apply to the screen space processing pipeline.
 */
class ScreenSpaceConfiguratorInterface : public GraphicsPipelineArgumentsInterface {
  public:
    ScreenSpaceConfiguratorInterface();
    virtual ~ScreenSpaceConfiguratorInterface();

    /**
     * @brief InputImages Returns the input images to the screen space processor. The input_images
     * array has been pre-allocated. Indexes the image by its descriptor binding.
     */
    virtual void InputImages(std::vector<VkImageView> *input_images) const;

    /**
     * @brief PushConstants A byte array containing push constant data of the screen space
     * processing pipeline.
     */
    virtual void PushConstants(std::vector<uint8_t> *push_constants) const;

    /**
     * @brief UniformBufferConstants
     * @param binding
     * @param push_constants
     */
    virtual void UniformBufferConstants(unsigned binding,
                                        std::vector<uint8_t> *uniform_buffer_constants) const;
};

/**
 * @brief The ScreenSpaceProcessorPipeline class A generic configurable screen space processing
 * graphics pipeline. To configure the pipeline, the client should supply arguments to the
 * constructor and pass a ScreenSpaceConfiguratorInterface as the argument while scheduling. Note,
 * it's a fully implemented concrete class.
 */
class ScreenSpaceProcessorPipeline final : public GraphicsPipelineInterface {
  public:
    /**
     * @brief ScreenSpaceProcessorPipeline Constructs a custom screen space processor.
     *
     * @param key A unique key assigned to this screen space processing pipeline.
     * @param fragment_shader The fragment shader to create the desired screen space processing
     * effect.
     * @param input_image_count The number of input images the screen space processor requires.
     * @param push_constant_size The number of bytes the screen space processor's push constants
     * requires.
     * @param output To receive output from the screen space processor.
     * @param transfer_context Transfer context.
     * @param vulkan_context
     */
    ScreenSpaceProcessorPipeline(PipelineKey const &key, std::string const &fragment_shader,
                                 unsigned input_image_count, unsigned push_constant_size,
                                 GraphicsPipelineOutputInterface *output,
                                 TransferContext *transfer_context, VulkanContext *vulkan_context);
    ~ScreenSpaceProcessorPipeline() override;

    PipelineKey Key() const override;

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *output, TransferContext *transfer_context,
                CommandBuffer *command_buffer) override;

  private:
    struct PostProcessorPipelineImpl;
    std::unique_ptr<PostProcessorPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_SCREEN_SPACE_PROCESSOR_H
