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

#include <cassert>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/command_buffer.h"

namespace e8 {

CommandBuffer::CommandBuffer(VulkanContext *context) : context_(context) {
    assert(context != nullptr);

    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    assert(VK_SUCCESS == vkAllocateCommandBuffers(context->device, &cmds_allocation_info, &buffer));
}

CommandBuffer::~CommandBuffer() {
    vkFreeCommandBuffers(context_->device, context_->command_pool, /*commandBufferCount=*/1,
                         &buffer);
}

std::unique_ptr<CommandBuffer> CreateCommandBuffer(VulkanContext *context) {
    return std::make_unique<CommandBuffer>(context);
}

void ResetCommandBuffer(CommandBuffer *command_buffer) {
    vkResetCommandBuffer(command_buffer->buffer, /*flags=*/0);
}

void BeginCommandBuffer(CommandBuffer *command_buffer) {
    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(command_buffer->buffer, &command_buffer_begin_info));
}

void EndCommandBuffer(CommandBuffer *command_buffer) {
    assert(VK_SUCCESS == vkEndCommandBuffer(command_buffer->buffer));
}

void SubmitCommandBuffer(CommandBuffer const &command_buffer,
                         std::vector<VkSemaphore> const &gpu_waits,
                         std::vector<VkSemaphore> const &gpu_signals, VkFence cpu_signal,
                         VulkanContext *context) {
    std::vector<VkPipelineStageFlags> wait_stages;
    wait_stages.reserve(gpu_waits.size());
    for (unsigned i = 0; i < gpu_waits.size(); ++i) {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        wait_stages.push_back(wait_stage);
    }

    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.pWaitSemaphores = gpu_waits.data();
    submit.waitSemaphoreCount = gpu_waits.size();
    submit.pWaitDstStageMask = wait_stages.data();
    submit.pSignalSemaphores = gpu_signals.data();
    submit.signalSemaphoreCount = gpu_signals.size();
    submit.pCommandBuffers = &command_buffer.buffer;
    submit.commandBufferCount = 1;

    assert(VK_SUCCESS ==
           vkQueueSubmit(context->graphics_queue, /*submitCount=*/1, &submit, cpu_signal));
}

} // namespace e8
