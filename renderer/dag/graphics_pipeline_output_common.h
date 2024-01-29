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

#ifndef ISLANDS_RENDERER_GRAHPICS_PIPELINE_OUTPUT_COMMON_H
#define ISLANDS_RENDERER_GRAHPICS_PIPELINE_OUTPUT_COMMON_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"
#include "renderer/dag/graphics_pipeline_output.h"

namespace e8 {

/**
 * @brief The SwapChainOutput class Stores the final rendering result.
 */
class SwapChainOutput final : public GraphicsPipelineOutputInterface {
  public:
    /**
     * @brief SwapChainPipelineOutput Constructs a swap chain output using the swap chain definition
     * from the Vulkan context.
     *
     * @param with_depth_buffer Specifiies if the output contains depth information.
     * @param context Contextual Vulkan handles.
     */
    SwapChainOutput(bool with_depth_buffer, VulkanContext *context);
    ~SwapChainOutput() override;

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
    struct SwapChainOutputImpl;
    std::unique_ptr<SwapChainOutputImpl> pimpl_;
};

/**
 * @brief The HdrColorOutput class Stores unormalized HDR linear RGBA color values.
 */
class HdrColorOutput final : public GraphicsPipelineOutputInterface {
  public:
    HdrColorOutput(unsigned width, unsigned height, bool with_depth_buffer, VulkanContext *context);
    ~HdrColorOutput() override;

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct HdrColorOutputImpl;
    std::unique_ptr<HdrColorOutputImpl> pimpl_;
};

/**
 * @brief The LdrColorOutput class Stores LDR color values in sRGB color space.
 */
class LdrColorOutput final : public GraphicsPipelineOutputInterface {
  public:
    LdrColorOutput(unsigned width, unsigned height, bool with_depth_buffer, VulkanContext *context);
    ~LdrColorOutput() override;

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct LdrColorOutputImpl;
    std::unique_ptr<LdrColorOutputImpl> pimpl_;
};

/**
 * @brief The FloatOutput class Stores 32-bit floating point values. An optional 32-bit depth
 * attachment can also be added.
 */
class FloatOutput final : public GraphicsPipelineOutputInterface {
  public:
    FloatOutput(unsigned width, unsigned height, bool with_depth_buffer, VulkanContext *context);
    ~FloatOutput() override;

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    struct FloatOutputImpl;
    std::unique_ptr<FloatOutputImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_GRAHPICS_PIPELINE_OUTPUT_COMMON_H
