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

#ifndef ISLANDS_RENDERER_PROMISE_H
#define ISLANDS_RENDERER_PROMISE_H

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

/**
 * @brief The GpuBarrier struct Stores a series of task semaphores. When later tasks are submitted
 * with waiting for this barrier, they will be blocked until all previous tasks merged in this
 * barrier are finished. It will clean up task semaphores and recycle task command buffers by the
 * end of its lifecycle.
 */
struct GpuPromise {
    /**
     * @brief GpuBarrier Constructs an empty barrier.
     * @param context Contextual Vulkan handles.
     */
    explicit GpuPromise(VulkanContext *context);

    /**
     * @brief GpuBarrier Constructs a barrier with one task.
     * @param task_signal The task's semaphore which will be signaled by the moment of completion
     * (when all the task_cmds finish running).
     * @param task_cmds The commands the task consists of.
     * @param context Contextual Vulkan handles.
     */
    GpuPromise(VkSemaphore task_signal, VkCommandBuffer task_cmds, VulkanContext *context);
    GpuPromise(GpuPromise const &) = delete;
    GpuPromise(GpuPromise &&other);
    ~GpuPromise();

    /**
     * @brief Merge Merges tasks from another barrier.
     */
    void Merge(std::unique_ptr<GpuPromise> &&other);

    // Tasks' signal.
    std::vector<VkSemaphore> tasks_signal;

    // Tasks' commands.
    std::vector<VkCommandBuffer> tasks_cmds;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PROMISE_H
