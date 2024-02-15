/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2023 Chifeng Wen {daviesx66@gmail.com}
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
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

uint64_t const kMaxSessionAndUsageCountDifference = 1000;

} // namespace

DagContext::DagContext(VulkanContext *context)
    : vulkan_context_(context), transfer_context_(TransferContext(context)), session_count_(0),
      in_session_(false) {
    assert(context != nullptr);
}

DagContext::~DagContext() { vkDeviceWaitIdle(vulkan_context_->device); }

DagContext::Session::Session(DagContext *dag_context) : self_(dag_context) {
    ++self_->session_count_;
    self_->in_session_ = true;
}

DagContext::Session::Session(Session &&other) {
    self_ = other.self_;
    other.self_ = nullptr;
}

DagContext::Session::~Session() {
    self_->in_session_ = false;

    for (auto it = self_->dag_ops_.begin(); it != self_->dag_ops_.end();) {
        auto &[_, dag_op_with_usage] = *it;

        if (self_->session_count_ - dag_op_with_usage.usage_count >
            kMaxSessionAndUsageCountDifference) {
            it = self_->dag_ops_.erase(it);
        } else {
            ++it;
        }
    }
}

DagContext::Session DagContext::CreateSession() { return Session(this); }

DagOperationInstance DagContext::WithOperation(DagOperationKey const &key,
                                               CreateDagOperationFn create_fn) {
    assert(in_session_);

    auto it = dag_ops_.find(key);
    if (it == dag_ops_.end()) {
        DagOperationWithUsage dag_with_usage;
        dag_with_usage.dag_op = create_fn(&transfer_context_, vulkan_context_);

        it = dag_ops_.insert(std::make_pair(key, std::move(dag_with_usage))).first;
    }

    auto &[_, dag_with_usage] = *it;
    ++dag_with_usage.usage_count;
    return dag_with_usage.dag_op.get();
}

DagContext::DagOperationKey CreateDagOperationKey(PipelineKey const &key,
                                                  unsigned const output_width,
                                                  unsigned const output_height) {
    return key + "_" + std::to_string(output_width) + "_" + std::to_string(output_height);
}

} // namespace e8
