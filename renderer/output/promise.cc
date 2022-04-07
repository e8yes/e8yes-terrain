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

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/output/promise.h"

namespace e8 {

GpuPromise::GpuPromise(VulkanContext *context) : context_(context) {}

GpuPromise::GpuPromise(VkSemaphore task_signal, VkCommandBuffer task_cmds, VulkanContext *context)
    : tasks_signal{task_signal}, tasks_cmds{task_cmds}, context_(context) {}

GpuPromise::GpuPromise(GpuPromise &&other) {
    tasks_signal = other.tasks_signal;
    tasks_cmds = other.tasks_cmds;
    context_ = other.context_;

    other.tasks_signal.clear();
    other.tasks_cmds.clear();
}

GpuPromise::~GpuPromise() {
    for (auto task_signal : tasks_signal) {
        vkDestroySemaphore(context_->device, task_signal, /*pAllocator=*/nullptr);
    }

    if (!tasks_cmds.empty()) {
        vkFreeCommandBuffers(context_->device, context_->command_pool, tasks_cmds.size(),
                             tasks_cmds.data());
    }
}

void GpuPromise::Merge(std::unique_ptr<GpuPromise> &&other) {
    for (auto signal : other->tasks_signal) {
        tasks_signal.push_back(signal);
    }
    other->tasks_signal.clear();

    for (auto cmds : other->tasks_cmds) {
        tasks_cmds.push_back(cmds);
    }
    other->tasks_cmds.clear();
}

} // namespace e8
