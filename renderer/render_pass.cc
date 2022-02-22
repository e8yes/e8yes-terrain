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
#include <functional>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "renderer/render_pass.h"
#include "renderer/vram.h"

namespace e8 {

GpuBarrier::GpuBarrier(VulkanContext *context) : context_(context) {}

GpuBarrier::GpuBarrier(VkSemaphore task_signal, VkCommandBuffer task_cmds, VulkanContext *context)
    : tasks_signal{task_signal}, tasks_cmds{task_cmds}, context_(context) {}

GpuBarrier::~GpuBarrier() {
    for (auto task_signal : tasks_signal) {
        vkDestroySemaphore(context_->device, task_signal, /*pAllocator=*/nullptr);
    }

    if (!tasks_cmds.empty()) {
        vkFreeCommandBuffers(context_->device, context_->command_pool, tasks_cmds.size(),
                             tasks_cmds.data());
    }
}

void GpuBarrier::Merge(std::unique_ptr<GpuBarrier> &&other) {
    for (auto signal : other->tasks_signal) {
        tasks_signal.push_back(signal);
    }
    other->tasks_signal.clear();

    for (auto cmds : other->tasks_cmds) {
        tasks_cmds.push_back(cmds);
    }
    other->tasks_cmds.clear();
}

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

VkCommandBuffer StartRenderPass(RenderPass const &render_pass, FrameBuffer const &frame_buffer,
                                VulkanContext *context) {
    // Allocates a new command buffer for storing commands from this render pass.
    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmds;
    assert(VK_SUCCESS == vkAllocateCommandBuffers(context->device, &cmds_allocation_info, &cmds));

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(cmds, &command_buffer_begin_info));

    // Sets the frame buffer and makes preparation for this render pass.
    VkRenderPassBeginInfo render_pass_begin_info{};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = render_pass.pass;
    render_pass_begin_info.framebuffer = frame_buffer.buffer;
    render_pass_begin_info.renderArea.extent.width = frame_buffer.width;
    render_pass_begin_info.renderArea.extent.height = frame_buffer.height;
    if (!frame_buffer.clear_values.empty()) {
        render_pass_begin_info.pClearValues = frame_buffer.clear_values.data();
        render_pass_begin_info.clearValueCount = frame_buffer.clear_values.size();
    }

    vkCmdBeginRenderPass(cmds, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    return cmds;
}

std::unique_ptr<GpuBarrier> FinishRenderPass(VkCommandBuffer cmds, GpuBarrier const &barrier,
                                             bool final, VulkanContext *context) {
    // Finalizes the render pass and command buffers.
    vkCmdEndRenderPass(cmds);
    assert(VK_SUCCESS == vkEndCommandBuffer(cmds));

    // For render pass synchronization.
    VkSemaphore done_signal;
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    assert(VK_SUCCESS == vkCreateSemaphore(context->device, &semaphore_info,
                                           /*pAllocator=*/nullptr, &done_signal));

    // Submit command buffers.
    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    if (!barrier.tasks_signal.empty()) {
        submit.pWaitSemaphores = barrier.tasks_signal.data();
        submit.waitSemaphoreCount = barrier.tasks_signal.size();
        submit.pWaitDstStageMask = &wait_stage;
    }
    submit.pSignalSemaphores = &done_signal;
    submit.signalSemaphoreCount = 1;
    submit.pCommandBuffers = &cmds;
    submit.commandBufferCount = 1;

    if (final) {
        assert(VK_SUCCESS == vkQueueSubmit(context->graphics_queue, /*submitCount=*/1, &submit,
                                           context->frame_fence));
    } else {
        assert(VK_SUCCESS == vkQueueSubmit(context->graphics_queue, /*submitCount=*/1, &submit,
                                           /*fence=*/nullptr));
    }

    return std::make_unique<GpuBarrier>(done_signal, cmds, context);
}

void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline, SetUniformsFn const &set_uniforms_fn,
                     GeometryVramTransfer *geo_vram, VkCommandBuffer cmds) {
    vkCmdBindPipeline(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    for (auto const &instance : drawables) {
        GeometryVramTransfer::UploadResult result = geo_vram->Upload(instance.drawable);
        if (!result.vertex_buffer.has_value() || !result.index_buffer.has_value()) {
            continue;
        }

        set_uniforms_fn(instance, cmds);

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(cmds, /*firstBinding=*/0, /*bindingCount=*/1,
                               &result.vertex_buffer->buffer,
                               /*pOffsets=*/&offset);
        vkCmdBindIndexBuffer(cmds, result.index_buffer->buffer, /*offset=*/0,
                             result.index_element_type);
        vkCmdDrawIndexed(cmds, instance.drawable->primitives().size() * 3,
                         /*instanceCount=*/1, /*firstIndex=*/0, /*vertexOffset=*/0,
                         /*firstInstance=*/0);
    }
}

} // namespace e8
