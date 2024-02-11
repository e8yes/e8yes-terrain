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
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

/**
 * @brief The CpuPromise struct Stores a CPU task fence. The fence gets signaled once the task is
 * complete. It will clean up task fence by the end of its lifecycle regardless of the task's
 * completion status.
 */
struct CpuPromise {
    /**
     * @brief CpuPromise Constructs a new CPU promise.
     *
     * @param context Contextual Vulkan handles.
     */
    explicit CpuPromise(VulkanContext *context);
    CpuPromise(CpuPromise const &) = delete;
    CpuPromise(CpuPromise &&other);
    ~CpuPromise();

    CpuPromise &operator=(CpuPromise &&other);

    /**
     * @brief Wait Waits for the promise to be signaled (resolved).
     */
    void Wait();

    /**
     * @brief Reset
     */
    void Reset();

    // The task's CPU signal.
    VkFence signal;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief The GpuPromise struct Stores a GPU task semaphore. The semaphore gets signaled once the
 * task is complete. It will clean up task semaphore and recycle task command buffers by the end of
 * its lifecycle regardless of the task's completion status.
 */
struct GpuPromise {
    /**
     * @brief GpuBarrier Constructs a GPU promise with the specified task commands.
     *
     * @param task_cmds Optional. The task's GPU commands.
     * @param context Contextual Vulkan handles.
     */
    explicit GpuPromise(VulkanContext *context);
    GpuPromise(GpuPromise const &) = delete;
    GpuPromise(GpuPromise &&other);
    ~GpuPromise();

    GpuPromise &operator=(GpuPromise &&other);

    /**
     * @brief Reset
     */
    void Reset();

    // Task's signal.
    VkSemaphore signal;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PROMISE_H
