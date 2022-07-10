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

#include <vulkan/vulkan.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "common/device.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief The PostProcessorConfiguratorInterface class For configuring what shader uniform setup to
 * apply to the post processing pipeline.
 */
class PostProcessorConfiguratorInterface : public CachedPipelineArgumentsInterface {
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
 * @brief The PostProcessorPipeline class A generic configurable post processing graphics pipeline.
 * To configure the pipeline, the client should supply arguments to the constructor and pass a
 * PostProcessorConfiguratorInterface as the argument while scheduling. Note, it's a
 * fully implemented concrete class.
 */
class PostProcessorPipeline : public CachedPipelineInterface {
   public:
    /**
     * @brief PostProcessorPipeline Constructs a custom post processor.
     *
     * @param key A unique key assigned to this post processing pipeline.
     * @param fragment_shader The fragment shader to create the desired post processing effect.
     * @param input_image_count The number of input images the post processor requires.
     * @param push_constant_size The number of bytes the post processor's push constants requires.
     * @param output To receive output from the post processor.
     * @param transfer_context Transfer context.
     */
    PostProcessorPipeline(PipelineKey const &key, std::string const &fragment_shader,
                          unsigned input_image_count, unsigned push_constant_size,
                          PipelineOutputInterface *output, TransferContext *transfer_context);
    ~PostProcessorPipeline() override;

    PipelineKey Key() const override;

    Fulfillment Launch(CachedPipelineArgumentsInterface const &generic_args,
                       std::vector<GpuPromise *> const &prerequisites,
                       unsigned completion_signal_count, PipelineOutputInterface *output) override;

   private:
    struct PostProcessorPipelineImpl;
    std::unique_ptr<PostProcessorPipelineImpl> pimpl_;
};

}  // namespace e8

#endif  // ISLANDS_RENDERER_PIPELINE_POST_PROCESSOR_H
