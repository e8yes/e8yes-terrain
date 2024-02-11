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
 * @brief The CommandBuffer class
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
 * @brief CreateCommandBuffer
 * @param context
 * @return
 */
std::unique_ptr<CommandBuffer> CreateCommandBuffer(VulkanContext *context);

/**
 * @brief ResetCommandBuffer
 */
void ResetCommandBuffer(CommandBuffer *command_buffer);

/**
 * @brief BeginCommandBuffer
 */
void BeginCommandBuffer(CommandBuffer *command_buffer);

/**
 * @brief EndCommandBuffer
 */
void EndCommandBuffer(CommandBuffer *command_buffer);

/**
 * @brief SubmitCommandBuffer
 * @param command_buffer
 * @param gpu_waits
 * @param gpu_signals
 * @param cpu_signal
 * @param context
 */
void SubmitCommandBuffer(CommandBuffer const &command_buffer,
                         std::vector<VkSemaphore> const &gpu_waits,
                         std::vector<VkSemaphore> const &gpu_signals, VkFence cpu_signal,
                         VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_COMMAND_BUFFER_H
