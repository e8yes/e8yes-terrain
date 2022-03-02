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

#include <cassert>
#include <chrono>
#include <memory>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/frame.h"
#include "renderer/pipeline_output.h"

namespace e8 {

StartFrameResult::StartFrameResult(VulkanContext *context)
    : acquire_swap_chain_image_barrier(context) {}

StartFrameResult::~StartFrameResult() {}

std::unique_ptr<StartFrameResult> StartFrame(VulkanContext *context) {
    assert(VK_SUCCESS == vkResetFences(context->device, /*fenceCount=*/1, &context->frame_fence));

    auto result = std::make_unique<StartFrameResult>(context);

    VkSemaphore done_signal;
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    assert(VK_SUCCESS == vkCreateSemaphore(context->device, &semaphore_info,
                                           /*pAllocator=*/nullptr, &done_signal));

    assert(VK_SUCCESS == vkAcquireNextImageKHR(context->device, context->swap_chain,
                                               /*timeout=*/UINT64_MAX, done_signal,
                                               /*fence=*/nullptr, &result->swap_chain_image_index));
    result->acquire_swap_chain_image_barrier.tasks_signal.push_back(done_signal);

    return result;
}

void EndFrame(GpuBarrier const &final_tasks, unsigned swap_chain_image_index,
              std::chrono::nanoseconds const &max_frame_duration, VulkanContext *context) {
    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pSwapchains = &context->swap_chain;
    present_info.swapchainCount = 1;
    present_info.pImageIndices = &swap_chain_image_index;
    if (!final_tasks.tasks_signal.empty()) {
        present_info.pWaitSemaphores = final_tasks.tasks_signal.data();
        present_info.waitSemaphoreCount = final_tasks.tasks_signal.size();
    }

    assert(VK_SUCCESS == vkQueuePresentKHR(context->present_queue, &present_info));

    assert(VK_SUCCESS == vkWaitForFences(context->device, /*fenceCount=*/1, &context->frame_fence,
                                         /*waitAll=*/VK_TRUE, max_frame_duration.count()));
}

} // namespace e8
