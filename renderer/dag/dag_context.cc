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

#include "renderer/dag/dag_context.h"

namespace e8 {
namespace {

uint64_t const kMaxSessionAndUsageCountDifference = 10;

} // namespace

DagContext::DagContext() : in_session_(false) {}

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

DagOperation *DagContext::WithOperation(const DagOperationKey &key,
                                        CreateDagOperationFn create_fn) {
    assert(in_session_);

    auto it = dag_ops_.find(key);
    if (it == dag_ops_.end()) {
        DagOperationWithUsage dag_with_usage;
        dag_with_usage.dag_op = create_fn();

        it = dag_ops_.insert(std::make_pair(key, std::move(dag_with_usage))).first;
    }

    auto &[_, dag_with_usage] = *it;
    ++dag_with_usage.usage_count;
    return dag_with_usage.dag_op.get();
}

} // namespace e8
