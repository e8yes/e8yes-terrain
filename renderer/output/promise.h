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
     * @brief Wait
     */
    void Wait();

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
    GpuPromise(VulkanContext *context);
    GpuPromise(VkCommandBuffer cmds, VulkanContext *context);
    GpuPromise(GpuPromise const &) = delete;
    GpuPromise(GpuPromise &&other);
    ~GpuPromise();

    GpuPromise &operator=(GpuPromise &&other);

    //
    VkCommandBuffer cmds;

    // Task's signal.
    VkSemaphore signal;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief The Fulfillment struct A promise-like abstraction of an asychronous GPU operation.
 */
struct Fulfillment {
    /**
     * @brief Fulfillment Constructs an empty fulfillment. Completion signal and child operations'
     * signal need to be assigned after the construction.
     *
     * @param cmds Optional. The GPU commands (task) that are running asynchronously. This class
     * manages the lifecycle of the command buffer.
     * @param context Contextual Vulkan handles.
     */
    Fulfillment(VkCommandBuffer cmds, VulkanContext *context);
    Fulfillment(Fulfillment const &) = delete;
    Fulfillment(Fulfillment &&other);
    ~Fulfillment();

    Fulfillment &operator=(Fulfillment &&other);

    // Optional. The GPU commands (task) that are running asynchronously.
    VkCommandBuffer cmds;

    // A host signal for notifying when the operation associated with this fulfillment is resolved.
    CpuPromise completion;

    // For signaling child operations. Each child operation should index into this array to find its
    // signal.
    std::vector<GpuPromise> child_operations_signal;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PROMISE_H
