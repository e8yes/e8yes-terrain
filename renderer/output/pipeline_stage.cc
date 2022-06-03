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
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"

namespace e8 {
namespace {

std::vector<GpuPromise *> ToGpuPromiseReference(
    std::optional<unsigned> child_id,
    std::unordered_map<PipelineStage::PipelineKey, std::vector<Fulfillment>> *fulfillment_cache) {
    std::vector<GpuPromise *> references;

    if (!child_id.has_value()) {
        return references;
    }

    for (auto &[_, fulfillments] : *fulfillment_cache) {
        for (auto &fulfillment : fulfillments) {
            assert(child_id < fulfillment.child_operations_signal.size());
            references.push_back(&fulfillment.child_operations_signal[*child_id]);
        }
    }

    return references;
}

void WaitForScheduleCompletions(
    std::unordered_map<PipelineStage::PipelineKey, std::vector<Fulfillment>> const
        &fulfillment_cache,
    VulkanContext *context) {
    std::vector<VkFence> fences;
    for (auto const &[_, fulfillments] : fulfillment_cache) {
        for (auto const &fulfillment : fulfillments) {
            fences.push_back(fulfillment.completion.signal);
        }
    }

    assert(VK_SUCCESS == vkWaitForFences(context->device, fences.size(), fences.data(),
                                         /*waitAll=*/VK_TRUE,
                                         /*timeout=*/std::numeric_limits<uint64_t>::max()));
}

} // namespace

struct PipelineStage::PipelineAndSchedule {
    PipelineAndSchedule(std::unique_ptr<CachedPipelineInterface> &&pipeline);
    PipelineAndSchedule(PipelineAndSchedule const &) = delete;
    PipelineAndSchedule(PipelineAndSchedule &&other);
    ~PipelineAndSchedule();

    PipelineAndSchedule &operator=(PipelineAndSchedule &&other);
    void ResetSchedule();

    std::unique_ptr<CachedPipelineInterface> pipeline;
    std::vector<PipelineStage *> parents;
    std::vector<ChildId> parent_child_ids;
    unsigned instance_count;
};

PipelineStage::PipelineAndSchedule::PipelineAndSchedule(
    std::unique_ptr<CachedPipelineInterface> &&pipeline)
    : pipeline(std::move(pipeline)), instance_count(0) {}

PipelineStage::PipelineAndSchedule::~PipelineAndSchedule() {}

PipelineStage::PipelineAndSchedule::PipelineAndSchedule(PipelineAndSchedule &&other) {
    *this = std::move(other);
}

PipelineStage::PipelineAndSchedule &
PipelineStage::PipelineAndSchedule::operator=(PipelineAndSchedule &&other) {
    std::swap(pipeline, other.pipeline);
    std::swap(parents, other.parents);
    std::swap(instance_count, other.instance_count);
    return *this;
}

void PipelineStage::PipelineAndSchedule::ResetSchedule() {
    parents.clear();
    parent_child_ids.clear();
    instance_count = 0;
}

struct PipelineStage::PipelineStageImpl {
    PipelineStageImpl(std::shared_ptr<PipelineOutputInterface> const &output);
    ~PipelineStageImpl();

    std::shared_ptr<PipelineOutputInterface> output;
    std::unordered_map<PipelineKey, PipelineAndSchedule> pipelines;
    unsigned child_count;
    std::unordered_map<PipelineStage::PipelineKey, std::vector<Fulfillment>> fulfillment_cache;
};

PipelineStage::PipelineStageImpl::PipelineStageImpl(
    std::shared_ptr<PipelineOutputInterface> const &output)
    : output(output), child_count(0) {}

PipelineStage::PipelineStageImpl::~PipelineStageImpl() {}

PipelineStage::PipelineStage(std::shared_ptr<PipelineOutputInterface> const &output)
    : pimpl_(std::make_unique<PipelineStageImpl>(output)) {}

PipelineStage::~PipelineStage() {}

PipelineStage::ChildId PipelineStage::AllocateChild() { return pimpl_->child_count++; }

std::unordered_set<PipelineStage *> PipelineStage::Parents() {
    std::unordered_set<PipelineStage *> parents;

    for (auto const &[_, pipeline_and_schedule] : pimpl_->pipelines) {
        for (auto parent : pipeline_and_schedule.parents) {
            parents.insert(parent);
        }
    }

    return parents;
}

void PipelineStage::LaunchSchedule(
    PipelineKey const &pipeline_key, PipelineAndSchedule const &pipeline_and_schedule,
    unsigned child_count, PipelineOutputInterface *output,
    std::unordered_map<PipelineKey, std::vector<Fulfillment>> *fulfillment_cache) {
    // Collects inputs.
    std::vector<PipelineOutputInterface *> inputs;
    std::vector<GpuPromise *> prerequisites;

    for (unsigned i = 0; i < pipeline_and_schedule.parents.size(); ++i) {
        inputs.push_back(pipeline_and_schedule.parents[i]->Output());
        std::vector<GpuPromise *> parent_promises =
            pipeline_and_schedule.parents[i]->LaunchSchedulesFor(
                pipeline_and_schedule.parent_child_ids[i]);

        for (auto &parent_promise : parent_promises) {
            prerequisites.emplace_back(std::move(parent_promise));
        }
    }

    // Runs the pipeline.
    for (unsigned i = 0; i < pipeline_and_schedule.instance_count; ++i) {
        Fulfillment fulfillment =
            pipeline_and_schedule.pipeline->Launch(i, inputs, prerequisites, child_count, output);

        (*fulfillment_cache)[pipeline_key].emplace_back(std::move(fulfillment));
    }
}

std::vector<GpuPromise *> PipelineStage::LaunchSchedulesFor(std::optional<ChildId> child_id) {
    if (!pimpl_->fulfillment_cache.empty()) {
        return ToGpuPromiseReference(child_id, &pimpl_->fulfillment_cache);
    }

    for (auto &[pipeline_key, pipeline_and_schedule] : pimpl_->pipelines) {
        this->LaunchSchedule(pipeline_key, pipeline_and_schedule, pimpl_->child_count,
                             pimpl_->output.get(), &pimpl_->fulfillment_cache);
    }

    return ToGpuPromiseReference(child_id, &pimpl_->fulfillment_cache);
}

void PipelineStage::Reset() {
    for (auto parent : this->Parents()) {
        parent->Reset();
    }

    for (auto &[_, pipeline_and_schedule] : pimpl_->pipelines) {
        pipeline_and_schedule.ResetSchedule();
    }

    pimpl_->fulfillment_cache.clear();
    pimpl_->child_count = 0;
}

CachedPipelineInterface *PipelineStage::WithPipeline(PipelineKey const &key,
                                                     CompilePipelineFn compile_fn) {
    auto it = pimpl_->pipelines.find(key);
    if (it != pimpl_->pipelines.end()) {
        return it->second.pipeline.get();
    }

    std::unique_ptr<CachedPipelineInterface> pipeline = compile_fn(pimpl_->output.get());
    it = pimpl_->pipelines.insert(std::make_pair(key, PipelineAndSchedule(std::move(pipeline))))
             .first;

    return it->second.pipeline.get();
}

void PipelineStage::Schedule(PipelineKey const &key, std::vector<PipelineStage *> const &parents,
                             unsigned instance_count) {
    auto it = pimpl_->pipelines.find(key);
    assert(it != pimpl_->pipelines.end());

    it->second.parent_child_ids.reserve(parents.size());
    for (auto parent : parents) {
        ChildId parent_child_id = parent->AllocateChild();
        it->second.parent_child_ids.push_back(parent_child_id);
    }

    it->second.parents = parents;
    it->second.instance_count = instance_count;
}

PipelineOutputInterface *PipelineStage::Output() { return pimpl_->output.get(); }

void PipelineStage::Fulfill(VulkanContext *context) {
    this->LaunchSchedulesFor(/*child_id=*/std::nullopt);

    for (auto parent : this->Parents()) {
        WaitForScheduleCompletions(parent->pimpl_->fulfillment_cache, context);
    }

    // Resets states for taking new schedules.
    this->Reset();
}

} // namespace e8
