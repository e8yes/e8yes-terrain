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
#include <limits>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/render_pass.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"

namespace e8 {

PipelineOutputInterface::PipelineOutputInterface(unsigned width, unsigned height,
                                                 VulkanContext *context)
    : width_(width), height_(height), context_(context) {}

PipelineOutputInterface::~PipelineOutputInterface() {}

unsigned PipelineOutputInterface::Width() const { return width_; }

unsigned PipelineOutputInterface::Height() const { return height_; }

void PipelineOutputInterface::Fulfill() {
    for (auto const &promise : cpu_promises_) {
        assert(VK_SUCCESS == vkWaitForFences(context_->device, /*fenceCount=*/1, &promise->signal,
                                             /*waitAll=*/VK_TRUE,
                                             /*timeout=*/std::numeric_limits<uint64_t>::max()));
    }

    gpu_promises_.clear();
    cpu_promises_.clear();
}

GpuPromise const *PipelineOutputInterface::Promise() const {
    if (gpu_promises_.empty()) {
        return nullptr;
    }
    return gpu_promises_.back().get();
}

void PipelineOutputInterface::AddWriter(std::unique_ptr<GpuPromise> &&gpu_promise,
                                        std::unique_ptr<CpuPromise> &&cpu_promise) {
    gpu_promises_.push_back(std::move(gpu_promise));
    cpu_promises_.push_back(std::move(cpu_promise));
}

} // namespace e8
