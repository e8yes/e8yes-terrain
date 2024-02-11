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
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/promise.h"

namespace e8 {

FrameResourceAllocator::FrameResourceAllocator(VulkanContext *context)
    : context_(context), frames_(context->swap_chain_images.size()) {}

FrameResourceAllocator::~FrameResourceAllocator() { vkDeviceWaitIdle(context_->device); }

void FrameResourceAllocator::SetSwapChainImageIndex(unsigned swap_chain_image_index) {
    assert(swap_chain_image_index < frames_.size());

    for (auto &cpu_promise : frames_[swap_chain_image_index].cpu_promises) {
        cpu_promise->Reset();
    }
    for (auto &gpu_promise : frames_[swap_chain_image_index].gpu_promises) {
        gpu_promise->Reset();
    }
    for (auto &command_buffer : frames_[swap_chain_image_index].command_buffers) {
        ResetCommandBuffer(command_buffer.get());
    }

    frames_[swap_chain_image_index].num_cpu_promises_allocated = 0;
    frames_[swap_chain_image_index].num_gpu_promises_allocated = 0;
    frames_[swap_chain_image_index].num_cmd_buffers_allocated = 0;
    swap_chain_image_index_ = swap_chain_image_index;
}

CpuPromise *FrameResourceAllocator::AllocateCpuPromise() {
    InFrameResources *current_frame = &frames_[swap_chain_image_index_];
    if (current_frame->num_cpu_promises_allocated == current_frame->cpu_promises.size()) {
        current_frame->cpu_promises.push_back(std::make_unique<CpuPromise>(context_));
    }
    return current_frame->cpu_promises[current_frame->num_cpu_promises_allocated++].get();
}

GpuPromise *FrameResourceAllocator::AllocateGpuPromise() {
    InFrameResources *current_frame = &frames_[swap_chain_image_index_];
    if (current_frame->num_gpu_promises_allocated == current_frame->gpu_promises.size()) {
        current_frame->gpu_promises.push_back(std::make_unique<GpuPromise>(context_));
    }
    return current_frame->gpu_promises[current_frame->num_gpu_promises_allocated++].get();
}

CommandBuffer *FrameResourceAllocator::AllocateCommandBuffer() {
    InFrameResources *current_frame = &frames_[swap_chain_image_index_];
    if (current_frame->num_cmd_buffers_allocated == current_frame->command_buffers.size()) {
        current_frame->command_buffers.push_back(std::make_unique<CommandBuffer>(context_));
    }
    return current_frame->command_buffers[current_frame->num_cmd_buffers_allocated++].get();
}

} // namespace e8
