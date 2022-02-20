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

#ifndef ISLANDS_RENDERER_RENDER_PASS_H
#define ISLANDS_RENDERER_RENDER_PASS_H

#include <chrono>
#include <functional>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/vram.h"

namespace e8 {

/**
 * @brief The GpuBarrier struct Stores a series of task semaphores. When later tasks are submitted
 * with waiting for this barrier, they will be blocked until all previous tasks merged in this
 * barrier are finished. It will clean up task semaphores and recycle task command buffers by the
 * end of its lifecycle.
 */
struct GpuBarrier {
    /**
     * @brief GpuBarrier Constructs an empty barrier.
     * @param context Contextual Vulkan handles.
     */
    explicit GpuBarrier(VulkanContext *context);

    /**
     * @brief GpuBarrier Constructs a barrier with one task.
     * @param task_signal The task's semaphore which will be signaled by the moment of completion
     * (when all the task_cmds finish running).
     * @param task_cmds The commands the task consists of.
     * @param context Contextual Vulkan handles.
     */
    GpuBarrier(VkSemaphore task_signal, VkCommandBuffer task_cmds, VulkanContext *context);
    ~GpuBarrier();

    GpuBarrier(GpuBarrier const &) = delete;

    /**
     * @brief Merge Merges tasks from another barrier.
     */
    void Merge(std::unique_ptr<GpuBarrier> &&other);

    // Tasks' signal.
    std::vector<VkSemaphore> tasks_signal;

    // Tasks' commands.
    std::vector<VkCommandBuffer> tasks_cmds;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

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
 * @param final_tasks The last few tasks which need to be finished before the frame can present.
 * @param max_frame_duration The greatest possible amount of time the frame's rendering process will
 * take before failing this function. This avoids the function from hanging without bound.
 * @param context Contextual Vulkan handles.
 */
void EndFrame(GpuBarrier const &final_tasks, unsigned swap_chain_image_index,
              std::chrono::nanoseconds const &max_frame_duration, VulkanContext *context);

/**
 * @brief StartRenderPass Allocates a command buffer and makes preparation for the render pass.
 *
 * @param pass The render pass to run.
 * @param frame_buffer The frame buffer to store the output of this render pass.
 * @param context Contextual Vulkan handles.
 * @return The allocated command buffer.
 */
VkCommandBuffer StartRenderPass(RenderPass const &pass, FrameBuffer const &frame_buffer,
                                VulkanContext *context);

/**
 * @brief FinishRenderPass Submits the command buffer to the graphics queue and returns a barrier
 * for this render pass.
 *
 * @param cmds The command buffer to be submitted.
 * @param barrier Previous tasks' barrier to allow this render pass to be placed after.
 * @param final Indicates if this render pass is the final task to wait for. This ensures
 * EndFrame() ends the frame only after this render pass is finished.
 * @param context Contextual Vulkan handles.
 * @return The task barrier for this render pass.
 */
std::unique_ptr<GpuBarrier> FinishRenderPass(VkCommandBuffer cmds, GpuBarrier const &barrier,
                                             bool final, VulkanContext *context);

// Represents a function which sets the value of uniform variables for drawing the particular
// drawable.
using SetUniformsFn =
    std::function<void(IslandsDrawableInstance const &drawable, VkCommandBuffer cmds)>;

/**
 * @brief RenderDrawables Renders an array of drawables with the specified graphics pipeline. Note,
 * it doesn't actually draw the drawables but only adds Vulkan commands to the command buffer.
 *
 * @param drawables The array of drawables to be rendered.
 * @param pipeline The graphics pipeline to use for the rendering.
 * @param set_uniforms_fn A custom function to set uniform variables for drawing a drawable.
 * @param geo_vram The geometry VRAM transferer.
 * @param cmds The command buffer to which draw commands will be added.
 */
void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline, SetUniformsFn const &set_uniforms_fn,
                     GeometryVramTransfer *geo_vram, VkCommandBuffer cmds);

} // namespace e8

#endif // ISLANDS_RENDERER_RENDER_PASS_H
