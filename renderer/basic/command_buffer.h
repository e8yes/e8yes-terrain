/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

#ifndef ISLANDS_RENDERER_COMMAND_BUFFER_H
#define ISLANDS_RENDERER_COMMAND_BUFFER_H

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

/**
 * @brief The CommandBuffer struct A command buffer records then submits a set of GPU commands. It
 * will clean up the command buffer resource by the end of its lifecycle.
 */
struct CommandBuffer {
    /**
     * @brief CommandBuffer Should be created only by calling CreateCommandBuffer().
     */
    CommandBuffer(VulkanContext *context);
    CommandBuffer(CommandBuffer const &) = delete;
    CommandBuffer(CommandBuffer &&) = delete;
    ~CommandBuffer();

    // A full Vulkan object storing information of a command buffer.
    VkCommandBuffer buffer;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateCommandBuffer Creates a command buffer for recording and submitting GPU commands.
 * This function will always return a valid CommandBuffer structure. Any failure occurs during the
 * command buffer creation will make it fail.
 *
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the CommandBuffer structure.
 */
std::unique_ptr<CommandBuffer> CreateCommandBuffer(VulkanContext *context);

/**
 * @brief ResetCommandBuffer Reset a command buffer to the initial state. Any primary command buffer
 * that is in the recording or executable state and has commandBuffer recorded into it, becomes
 * invalid.
 */
void ResetCommandBuffer(CommandBuffer *command_buffer);

/**
 * @brief BeginCommandBuffer Start recording a command buffer.
 */
void BeginCommandBuffer(CommandBuffer *command_buffer);

/**
 * @brief EndCommandBuffer Finish recording a command buffer.
 */
void EndCommandBuffer(CommandBuffer *command_buffer);

/**
 * @brief SubmitCommandBuffer Submits a sequence of semaphores or command buffers to the graphics
 * queue. The device can only start running the submitted commands after receiving all the gpu_waits
 * signals. The commands are guarenteed to be executed once the gpu_signals and cpu_signal are sent.
 *
 * @param command_buffer The command buffer to be submitted.
 * @param gpu_waits The GPU signals to be waiting for before the start of the commands execution.
 * @param gpu_signals The set of GPU signals to be sent on the completion of the commands execution.
 * @param cpu_signal The CPU signal to be sent on the completion of the commands execution.
 * @param context Contextual Vulkan handles.
 */
void SubmitCommandBuffer(CommandBuffer const &command_buffer,
                         std::vector<VkSemaphore> const &gpu_waits,
                         std::vector<VkSemaphore> const &gpu_signals, VkFence cpu_signal,
                         VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_COMMAND_BUFFER_H
