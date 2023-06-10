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

#ifndef ISLANDS_RENDERER_GRAPHICS_PIPELINE_OUTPUT_H
#define ISLANDS_RENDERER_GRAPHICS_PIPELINE_OUTPUT_H

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"

namespace e8 {

/**
 * @brief The GraphicsPipelineOutputInterface class Represents the output of a graphics pipeline.
 */
class GraphicsPipelineOutputInterface {
  public:
    GraphicsPipelineOutputInterface(unsigned width, unsigned height);
    virtual ~GraphicsPipelineOutputInterface();

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

  private:
    // The width of the rendered image.
    unsigned width_;

    // The height of the rendered image.
    unsigned height_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_GRAPHICS_PIPELINE_OUTPUT_H
