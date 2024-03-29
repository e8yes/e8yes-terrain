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
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/promise.h"
#include "renderer/transfer/context.h"

namespace e8 {

struct DagOperation::ParentStage {
    DagOperation *parent;
    unsigned child_id;
};

struct DagOperation::PipelineSchedule {
    std::unique_ptr<GraphicsPipelineArgumentsInterface> args;
    std::vector<ParentStage> dependent_parents;
};

struct DagOperation::PipelineAndSchedules {
    PipelineAndSchedules(std::unique_ptr<GraphicsPipelineInterface> &&pipeline)
        : pipeline(std::move(pipeline)) {}

    PipelineAndSchedules(PipelineAndSchedules const &) = delete;

    PipelineAndSchedules(PipelineAndSchedules &&other) { *this = std::move(other); }

    ~PipelineAndSchedules() = default;

    PipelineAndSchedules &operator=(PipelineAndSchedules &&other) {
        std::swap(pipeline, other.pipeline);
        std::swap(schedules, other.schedules);
        return *this;
    }

    void ResetSchedules() { schedules.clear(); }

    std::unique_ptr<GraphicsPipelineInterface> pipeline;
    std::vector<PipelineSchedule> schedules;
};

struct DagOperation::DagOperationImpl {
    DagOperationImpl(std::shared_ptr<GraphicsPipelineOutputInterface> const &output,
                     TransferContext *transfer_context, VulkanContext *vulkan_context)
        : output(output), transfer_context_(transfer_context), vulkan_context_(vulkan_context),
          num_child_stages(0) {}

    ~DagOperationImpl() = default;

    std::shared_ptr<GraphicsPipelineOutputInterface> output;
    TransferContext *transfer_context_;
    VulkanContext *vulkan_context_;
    std::unordered_map<PipelineKey, PipelineAndSchedules> pipelines;
    unsigned num_child_stages;
    std::vector<Fulfillment> fulfillment_cache;
};

DagOperation::DagOperation(std::shared_ptr<GraphicsPipelineOutputInterface> const &output,
                           TransferContext *transfer_context, VulkanContext *vulkan_context)
    : pimpl_(std::make_unique<DagOperationImpl>(output, transfer_context, vulkan_context)) {}

DagOperation::~DagOperation() {}

DagOperation::Fulfillment::Fulfillment(CpuPromise *completion,
                                       std::vector<GpuPromise *> child_operations_signal)
    : completion(completion), child_operations_signal(child_operations_signal) {}

DagOperation::ChildId DagOperation::AllocateChild() { return pimpl_->num_child_stages++; }

std::unordered_set<DagOperation *> DagOperation::UniqueParents() {
    std::unordered_set<DagOperation *> parents;

    for (auto const &[_, pipeline_and_schedules] : pimpl_->pipelines) {
        for (auto const &schedule : pipeline_and_schedules.schedules) {
            for (auto &parent : schedule.dependent_parents) {
                parents.insert(parent.parent);
            }
        }
    }

    return parents;
}

std::vector<GpuPromise *>
DagOperation::PickUpChildOperationsSignal(std::optional<unsigned> child_id) const {
    std::vector<GpuPromise *> references;

    if (!child_id.has_value()) {
        return references;
    }

    for (auto &fulfillment : pimpl_->fulfillment_cache) {
        assert(child_id < fulfillment.child_operations_signal.size());
        references.push_back(fulfillment.child_operations_signal[*child_id]);
    }

    return references;
}

DagOperation::Fulfillment
DagOperation::LaunchSchedule(GraphicsPipelineInterface *pipeline, PipelineSchedule const &schedule,
                             unsigned child_count, GraphicsPipelineOutputInterface *output,
                             FrameResourceAllocator *frame_resource_allocator,
                             VulkanContext *context) {
    // Runs the prerequisites parent stages.
    std::vector<GpuPromise *> prerequisites;

    for (auto const &dependent_parent : schedule.dependent_parents) {
        std::vector<GpuPromise *> parent_promises = dependent_parent.parent->LaunchSchedulesAs(
            dependent_parent.child_id, frame_resource_allocator, context);

        for (auto &parent_promise : parent_promises) {
            prerequisites.emplace_back(std::move(parent_promise));
        }
    }

    // Runs the pipeline.
    CommandBuffer *command_buffer = frame_resource_allocator->AllocateCommandBuffer();
    BeginCommandBuffer(command_buffer);
    pipeline->Launch(*schedule.args, output, pimpl_->transfer_context_, command_buffer);
    EndCommandBuffer(command_buffer);

    std::vector<VkSemaphore> waits;
    waits.reserve(prerequisites.size());
    for (auto prerequisite : prerequisites) {
        waits.push_back(prerequisite->signal);
    }

    std::vector<VkSemaphore> signals;
    std::vector<GpuPromise *> child_operations_signal;
    signals.reserve(child_count);
    child_operations_signal.reserve(child_count);
    for (unsigned i = 0; i < child_count; ++i) {
        GpuPromise *promise = frame_resource_allocator->AllocateGpuPromise();
        signals.push_back(promise->signal);
        child_operations_signal.push_back(promise);
    }
    CpuPromise *completion = frame_resource_allocator->AllocateCpuPromise();

    SubmitCommandBuffer(*command_buffer, waits, signals, completion->signal, context);
    return Fulfillment(completion, child_operations_signal);
}

std::vector<GpuPromise *>
DagOperation::LaunchSchedulesAs(std::optional<ChildId> child_id,
                                FrameResourceAllocator *frame_resource_allocator,
                                VulkanContext *context) {
    if (!pimpl_->fulfillment_cache.empty()) {
        return this->PickUpChildOperationsSignal(child_id);
    }

    for (auto &[pipeline_key, pipeline_and_schedules] : pimpl_->pipelines) {
        for (auto const &schedule : pipeline_and_schedules.schedules) {
            Fulfillment fulfillment = this->LaunchSchedule(
                pipeline_and_schedules.pipeline.get(), schedule, pimpl_->num_child_stages,
                pimpl_->output.get(), frame_resource_allocator, context);
            pimpl_->fulfillment_cache.push_back(std::move(fulfillment));
        }
    }

    return this->PickUpChildOperationsSignal(child_id);
}

void DagOperation::Reset() {
    for (auto parent : this->UniqueParents()) {
        parent->Reset();
    }

    for (auto &[_, pipeline_and_schedules] : pimpl_->pipelines) {
        pipeline_and_schedules.ResetSchedules();
    }

    pimpl_->fulfillment_cache.clear();
    pimpl_->num_child_stages = 0;
}

GraphicsPipelineInterface *DagOperation::WithPipeline(PipelineKey const &key,
                                                      CompilePipelineFn compile_fn) {
    auto it = pimpl_->pipelines.find(key);
    if (it != pimpl_->pipelines.end()) {
        return it->second.pipeline.get();
    }

    std::unique_ptr<GraphicsPipelineInterface> pipeline =
        compile_fn(pimpl_->output.get(), pimpl_->transfer_context_, pimpl_->vulkan_context_);
    it = pimpl_->pipelines.insert(std::make_pair(key, PipelineAndSchedules(std::move(pipeline))))
             .first;

    return it->second.pipeline.get();
}

void DagOperation::Schedule(GraphicsPipelineInterface const *pipeline,
                            std::unique_ptr<GraphicsPipelineArgumentsInterface> &&args,
                            std::vector<DagOperation *> const &parents) {
    // Forms a schedule.
    PipelineSchedule schedule;
    schedule.args = std::move(args);

    schedule.dependent_parents.reserve(parents.size());
    for (auto parent : parents) {
        if (parent == nullptr) {
            continue;
        }

        ParentStage parent_stage;
        parent_stage.child_id = parent->AllocateChild();
        parent_stage.parent = parent;

        schedule.dependent_parents.push_back(parent_stage);
    }

    // Inserts the schedule.
    auto it = pimpl_->pipelines.find(pipeline->Key());
    assert(it != pimpl_->pipelines.end());

    auto &[_, pipeline_and_schedules] = *it;
    pipeline_and_schedules.schedules.emplace_back(std::move(schedule));
}

GraphicsPipelineOutputInterface const *DagOperation::Output() const { return pimpl_->output.get(); }

GraphicsPipelineOutputInterface *DagOperation::Output() { return pimpl_->output.get(); }

std::vector<GpuPromise *> DagOperation::Fulfill(bool wait,
                                                FrameResourceAllocator *frame_resource_allocator) {
    std::vector<GpuPromise *> signals = this->LaunchSchedulesAs(
        /*child_id=*/std::nullopt, frame_resource_allocator, pimpl_->vulkan_context_);

    if (wait) {
        std::vector<VkFence> fences;
        for (auto const &fulfillment : pimpl_->fulfillment_cache) {
            fences.push_back(fulfillment.completion->signal);
        }

        assert(VK_SUCCESS == vkWaitForFences(pimpl_->vulkan_context_->device, fences.size(),
                                             fences.data(),
                                             /*waitAll=*/VK_TRUE,
                                             /*timeout=*/std::numeric_limits<uint64_t>::max()));
    }

    // Resets states for taking new schedules.
    this->Reset();

    return signals;
}

} // namespace e8
