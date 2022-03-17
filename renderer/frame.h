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

#ifndef ISLANDS_RENDERER_FRAME_H
#define ISLANDS_RENDERER_FRAME_H

#include <chrono>
#include <memory>

#include "renderer/context.h"
#include "renderer/pipeline_output.h"

namespace e8 {

/**
 * @brief The StartFrameResult struct Stores information about the swap chain image on which a new
 * frame of rendering process will draw.
 */
struct StartFrameResult {
    /**
     * @brief StartFrameResult Should be created only by calling StartFrame().
     */
    explicit StartFrameResult(VulkanContext *context);
    ~StartFrameResult();

    StartFrameResult(StartFrameResult const &) = delete;
    StartFrameResult(StartFrameResult &&) = delete;

    // When the swap chain image indexed by the swap_chain_image_index becomes available, the
    // barrier will be signaled.
    GpuBarrier acquire_swap_chain_image_barrier;

    // The index of the swap chain image which will soon be available for presentation.
    unsigned swap_chain_image_index;
};

/**
 * @brief StartFrame Makes preparation for a new frame.
 */
std::unique_ptr<StartFrameResult> StartFrame(VulkanContext *context);

/**
 * @brief EndFrame Marks the completion of a frame's rendering process.
 *
 * @param final_ouput The last output to be fulfilled before the frame can present.
 * @param max_frame_duration The greatest possible amount of time the frame's rendering process will
 * take before failing this function. This avoids the function from hanging without bound.
 * @param context Contextual Vulkan handles.
 */
void EndFrame(PipelineOutputInterface *final_ouput, unsigned swap_chain_image_index,
              std::chrono::nanoseconds const &max_frame_duration, VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_FRAME_H
