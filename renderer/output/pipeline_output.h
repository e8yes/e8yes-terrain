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
    PipelineOutputInterface(unsigned width, unsigned height, VulkanContext *context);
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
    virtual std::vector<FrameBufferAttachment const *> ColorAttachments() const = 0;

    /**
     * @brief DepthAttachment Returns the depth attachment image, if there is any from the output.
     */
    virtual FrameBufferAttachment const *DepthAttachment() const = 0;

    /**
     * @brief Width The width, in pixels, of the pipeline output.
     */
    unsigned Width() const;

    /**
     * @brief Height The height, in pixels, of the pipeline output.
     */
    unsigned Height() const;

    /**
     * @brief Fulfill Waits until either the output is fulfilled. The underlying promises will be
     * cleared out.
     */
    void Fulfill();

    /**
     * @brief Promise Gets the current GPU promises associated with the pipeline output. It's
     * nullable.
     */
    GpuPromise const *Promise() const;

    /**
     * @brief AddWriter Adds a writer task to the output.
     */
    void AddWriter(std::unique_ptr<GpuPromise> &&gpu_promise,
                   std::unique_ptr<CpuPromise> &&cpu_promise);

  private:
    // The width of the rendered image.
    unsigned width_;

    // The height of the rendered image.
    unsigned height_;

    // Writer tasks' GPU promise.
    std::vector<std::unique_ptr<GpuPromise>> gpu_promises_;

    // Writer tasks' CPU promise.
    std::vector<std::unique_ptr<CpuPromise>> cpu_promises_;

    // Contextual Vulkan handles.
    VulkanContext *context_;
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
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
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

/**
 * @brief The UnboundedColorPipelineOutput class Stores unormalized RGBA color values.
 */
class UnboundedColorPipelineOutput : public PipelineOutputInterface {
  public:
    UnboundedColorPipelineOutput(unsigned width, unsigned height, bool with_depth_buffer,
                                 VulkanContext *context);
    ~UnboundedColorPipelineOutput() override;

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct UnboundedColorPipelineOutputImpl;
    std::unique_ptr<UnboundedColorPipelineOutputImpl> pimpl_;
};

/**
 * @brief The LdrColorPipelineOutput class Stores LDR color values in sRGB color space.
 */
class LdrColorPipelineOutput : public PipelineOutputInterface {
  public:
    LdrColorPipelineOutput(unsigned width, unsigned height, bool with_depth_buffer,
                           VulkanContext *context);
    ~LdrColorPipelineOutput() override;

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct LdrColorPipelineOutputImpl;
    std::unique_ptr<LdrColorPipelineOutputImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_OUTPUT_H
