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

#ifndef ISLANDS_RENDERER_COMMON_OUTPUT_H
#define ISLANDS_RENDERER_COMMON_OUTPUT_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"
#include "renderer/output/pipeline_output.h"

namespace e8 {

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
     * @brief SwapChainImageIndex Gets the current swap chain image index (Returns the value set
     * through SetSwapChainImageIndex()).
     */
    unsigned SwapChainImageIndex() const;

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
 * @brief The HdrColorPipelineOutput class Stores unormalized HDR linear RGBA color values.
 */
class HdrColorPipelineOutput : public PipelineOutputInterface {
  public:
    HdrColorPipelineOutput(unsigned width, unsigned height, bool with_depth_buffer,
                           VulkanContext *context);
    ~HdrColorPipelineOutput() override;

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

#endif // ISLANDS_RENDERER_COMMON_OUTPUT_H
