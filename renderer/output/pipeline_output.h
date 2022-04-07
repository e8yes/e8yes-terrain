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

#ifndef ISLANDS_RENDERER_PIPELINE_OUTPUT_H
#define ISLANDS_RENDERER_PIPELINE_OUTPUT_H

#include <chrono>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"
#include "renderer/output/promise.h"

namespace e8 {

/**
 * @brief The PipelineOutputInterface class Represents the output of a graphics pipeline.
 */
class PipelineOutputInterface {
  public:
    PipelineOutputInterface(VulkanContext *context);
    virtual ~PipelineOutputInterface();

    /**
     * @brief GetFrameBuffer Returns the current frame buffer object.
     */
    virtual FrameBuffer *GetFrameBuffer() const = 0;

    /**
     * @brief GetRenderPass Returns the render pass tied to the output.
     */
    virtual RenderPass const &GetRenderPass() const = 0;

    /**
     * @brief ColorAttachment Returns the color attachment image, if there is any from the output.
     */
    virtual FrameBufferAttachment const *ColorAttachment() const = 0;

    /**
     * @brief DepthAttachment Returns the depth attachment image, if there is any from the output.
     */
    virtual FrameBufferAttachment const *DepthAttachment() const = 0;

    /**
     * @brief RequireFence Returns an unsignaled fence of this output.
     */
    VkFence AcquireFence();

    /**
     * @brief Fulfill Waits until either the output is fulfilled or gets timed out. If the wait is
     * timed out, this function will fail.
     */
    void Fulfill(std::chrono::nanoseconds const &timeout);

  public:
    // The width of the rendered image.
    unsigned width;

    // The height of the rendered image.
    unsigned height;

    // Barrier of the rendering task. Semaphores in this barrier must all be signaled before the
    // output can be read or used again.
    std::unique_ptr<GpuPromise> barrier;

    // A CPU fence which gets signaled when the output is fulfilled.
    VkFence fence;

    // Contextual Vulkan handles.
    VulkanContext *context;
};

/**
 * @brief The SwapChainPipelineOutput class Stores the final rendering result.
 */
class SwapChainPipelineOutput : public PipelineOutputInterface {
  public:
    /**
     * @brief SwapChainPipelineOutput Constructs a swap chain output using the swap chain definition
     * from the Vulkan context.
     *
     * @param with_depth_buffer Specifiies if the output contains depth information.
     * @param context Contextual Vulkan handles.
     */
    SwapChainPipelineOutput(bool with_depth_buffer, VulkanContext *context);
    ~SwapChainPipelineOutput() override;

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    FrameBufferAttachment const *ColorAttachment() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

    /**
     * @brief SetSwapChainImageIndex Sets the swap chain image to be used for storing rendering
     * output. The class functions will fail if the swap chain image index isn't set prior to
     * running a pipeline.
     */
    void SetSwapChainImageIndex(unsigned swap_chain_image_index);

  private:
    struct SwapChainPipelineOutputImpl;
    std::unique_ptr<SwapChainPipelineOutputImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_OUTPUT_H
