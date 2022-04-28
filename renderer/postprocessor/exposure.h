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

#ifndef ISLANDS_RENDERER_EXPOSURE_H
#define ISLANDS_RENDERER_EXPOSURE_H

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The LogLuminanceOutput class Stores the logarithmic luminance value of each radiance
 * pixel.
 */
class LogLuminancePipelineOutput : public PipelineOutputInterface {
  public:
    LogLuminancePipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~LogLuminancePipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    std::unique_ptr<FrameBufferAttachment> color_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

/**
 * @brief The ExposureEstimationPipelineOutput class Stores the logarithmic lumiance average in a
 * 1x1 image as the exposure of the radiance map.
 */
class ExposureEstimationPipelineOutput : public PipelineOutputInterface {
  public:
    explicit ExposureEstimationPipelineOutput(VulkanContext *context);
    ~ExposureEstimationPipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    std::unique_ptr<FrameBufferAttachment> color_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
};

/**
 * @brief The ExposureEstimationPipeline class Estimates the exposure of a radiance map.
 */
class ExposureEstimationPipeline {
  public:
    ExposureEstimationPipeline(DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~ExposureEstimationPipeline();

    /**
     * @brief Run Transforms the specified radiance map into a logarithmic luminance map and
     * computes its average.
     */
    void Run(UnboundedColorPipelineOutput const &radiance,
             LogLuminancePipelineOutput *log_luminance_output,
             ExposureEstimationPipelineOutput *exposure_output);

  private:
    struct ExposureEstimationPipelineImpl;

    DescriptorSetAllocator *desc_set_allocator_;
    VulkanContext *context_;
    LogLuminancePipelineOutput *current_output_;
    std::unique_ptr<ExposureEstimationPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_EXPOSURE_H
