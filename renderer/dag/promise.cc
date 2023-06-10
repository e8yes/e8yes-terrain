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

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/dag/promise.h"

namespace e8 {

CpuPromise::CpuPromise(VulkanContext *context) : signal(VK_NULL_HANDLE), context_(context) {
    if (context == nullptr) {
        return;
    }

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    assert(VK_SUCCESS ==
           vkCreateFence(context->device, &fence_info, /*pAllocator=*/nullptr, &signal));
}

CpuPromise::~CpuPromise() {
    if (signal != VK_NULL_HANDLE) {
        vkDestroyFence(context_->device, signal, /*pAllocator=*/nullptr);
    }
}

CpuPromise::CpuPromise(CpuPromise &&other) : signal(VK_NULL_HANDLE), context_(nullptr) {
    *this = std::move(other);
}

CpuPromise &CpuPromise::operator=(CpuPromise &&other) {
    std::swap(signal, other.signal);
    context_ = other.context_;
    return *this;
}

void CpuPromise::Wait() {
    assert(VK_SUCCESS == vkWaitForFences(context_->device, /*fenceCount=*/1, &signal,
                                         /*waitAll=*/VK_TRUE,
                                         /*timeout=*/std::numeric_limits<uint64_t>::max()));
}

GpuPromise::GpuPromise(VulkanContext *context) : context_(context) {
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    assert(VK_SUCCESS == vkCreateSemaphore(context->device, &semaphore_info,
                                           /*pAllocator=*/nullptr, &signal));
}

GpuPromise::GpuPromise(GpuPromise &&other) : signal(VK_NULL_HANDLE), context_(nullptr) {
    *this = std::move(other);
}

GpuPromise::~GpuPromise() {
    if (signal != VK_NULL_HANDLE) {
        vkDestroySemaphore(context_->device, signal, /*pAllocator=*/nullptr);
    }
}

GpuPromise &GpuPromise::operator=(GpuPromise &&other) {
    std::swap(signal, other.signal);
    context_ = other.context_;
    return *this;
}

Fulfillment::Fulfillment(VkCommandBuffer cmds, VulkanContext *context)
    : cmds(cmds), completion(context), context_(context) {}

Fulfillment::Fulfillment(Fulfillment &&other)
    : cmds(VK_NULL_HANDLE), completion(/*context=*/nullptr), context_(nullptr) {
    *this = std::move(other);
}

Fulfillment &Fulfillment::operator=(Fulfillment &&other) {
    std::swap(cmds, other.cmds);
    std::swap(completion, other.completion);
    std::swap(child_operations_signal, other.child_operations_signal);
    context_ = other.context_;
    return *this;
}

Fulfillment::~Fulfillment() {
    if (cmds != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(context_->device, context_->command_pool, /*commandBufferCount=*/1,
                             &cmds);
    }
}

} // namespace e8
