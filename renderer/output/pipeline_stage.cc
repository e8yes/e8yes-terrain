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

std::vector<GpuPromise *> ToGpuPromiseReference(std::optional<unsigned> child_id,
                                                std::vector<Fulfillment> *fulfillment_cache) {
    std::vector<GpuPromise *> references;

    if (!child_id.has_value()) {
        return references;
    }

    for (auto &fulfillment : *fulfillment_cache) {
        assert(child_id < fulfillment.child_operations_signal.size());
        references.push_back(&fulfillment.child_operations_signal[*child_id]);
    }

    return references;
}

void WaitForStageCompletion(std::vector<Fulfillment> const &fulfillment_cache,
                            VulkanContext *context) {
    std::vector<VkFence> fences;
    for (auto const &fulfillment : fulfillment_cache) {
        fences.push_back(fulfillment.completion.signal);
    }

    assert(VK_SUCCESS == vkWaitForFences(context->device, fences.size(), fences.data(),
                                         /*waitAll=*/VK_TRUE,
                                         /*timeout=*/std::numeric_limits<uint64_t>::max()));
}

} // namespace

struct PipelineStage::ParentStage {
    PipelineStage *parent;
    unsigned child_id;
};

struct PipelineStage::PipelineSchedule {
    std::unique_ptr<CachedPipelineArgumentsInterface> args;
    std::vector<ParentStage> dependent_parents;
};

struct PipelineStage::PipelineAndSchedules {
    PipelineAndSchedules(std::unique_ptr<CachedPipelineInterface> &&pipeline);
    PipelineAndSchedules(PipelineAndSchedules const &) = delete;
    PipelineAndSchedules(PipelineAndSchedules &&other);
    ~PipelineAndSchedules();

    PipelineAndSchedules &operator=(PipelineAndSchedules &&other);
    void ResetSchedules();

    std::unique_ptr<CachedPipelineInterface> pipeline;
    std::vector<PipelineSchedule> schedules;
};

PipelineStage::PipelineAndSchedules::PipelineAndSchedules(
    std::unique_ptr<CachedPipelineInterface> &&pipeline)
    : pipeline(std::move(pipeline)) {}

PipelineStage::PipelineAndSchedules::~PipelineAndSchedules() {}

PipelineStage::PipelineAndSchedules::PipelineAndSchedules(PipelineAndSchedules &&other) {
    *this = std::move(other);
}

PipelineStage::PipelineAndSchedules &
PipelineStage::PipelineAndSchedules::operator=(PipelineAndSchedules &&other) {
    std::swap(pipeline, other.pipeline);
    std::swap(schedules, other.schedules);
    return *this;
}

void PipelineStage::PipelineAndSchedules::ResetSchedules() { schedules.clear(); }

struct PipelineStage::PipelineStageImpl {
    PipelineStageImpl(std::shared_ptr<PipelineOutputInterface> const &output);
    ~PipelineStageImpl();

    std::shared_ptr<PipelineOutputInterface> output;
    std::unordered_map<PipelineKey, PipelineAndSchedules> pipelines;
    unsigned num_child_stages;
    std::vector<Fulfillment> fulfillment_cache;
};

PipelineStage::PipelineStageImpl::PipelineStageImpl(
    std::shared_ptr<PipelineOutputInterface> const &output)
    : output(output), num_child_stages(0) {}

PipelineStage::PipelineStageImpl::~PipelineStageImpl() {}

PipelineStage::PipelineStage(std::shared_ptr<PipelineOutputInterface> const &output)
    : pimpl_(std::make_unique<PipelineStageImpl>(output)) {}

PipelineStage::~PipelineStage() {}

PipelineStage::ChildId PipelineStage::AllocateChild() { return pimpl_->num_child_stages++; }

std::unordered_set<PipelineStage *> PipelineStage::UniqueParents() {
    std::unordered_set<PipelineStage *> parents;

    for (auto const &[_, pipeline_and_schedules] : pimpl_->pipelines) {
        for (auto const &schedule : pipeline_and_schedules.schedules) {
            for (auto &parent : schedule.dependent_parents) {
                parents.insert(parent.parent);
            }
        }
    }

    return parents;
}

Fulfillment PipelineStage::LaunchSchedule(CachedPipelineInterface *pipeline,
                                          PipelineSchedule const &schedule, unsigned child_count,
                                          PipelineOutputInterface *output) {
    // Runs the prerequisites parent stages.
    std::vector<GpuPromise *> prerequisites;

    for (auto const &dependent_parent : schedule.dependent_parents) {
        std::vector<GpuPromise *> parent_promises =
            dependent_parent.parent->LaunchSchedulesAs(dependent_parent.child_id);

        for (auto &parent_promise : parent_promises) {
            prerequisites.emplace_back(std::move(parent_promise));
        }
    }

    // Runs the pipeline.
    return pipeline->Launch(*schedule.args, prerequisites, child_count, output);
}

std::vector<GpuPromise *> PipelineStage::LaunchSchedulesAs(std::optional<ChildId> child_id) {
    if (!pimpl_->fulfillment_cache.empty()) {
        return ToGpuPromiseReference(child_id, &pimpl_->fulfillment_cache);
    }

    for (auto &[pipeline_key, pipeline_and_schedules] : pimpl_->pipelines) {
        for (auto const &schedule : pipeline_and_schedules.schedules) {
            Fulfillment fulfillment =
                this->LaunchSchedule(pipeline_and_schedules.pipeline.get(), schedule,
                                     pimpl_->num_child_stages, pimpl_->output.get());
            pimpl_->fulfillment_cache.emplace_back(std::move(fulfillment));
        }
    }

    return ToGpuPromiseReference(child_id, &pimpl_->fulfillment_cache);
}

void PipelineStage::Reset() {
    for (auto parent : this->UniqueParents()) {
        parent->Reset();
    }

    for (auto &[_, pipeline_and_schedules] : pimpl_->pipelines) {
        pipeline_and_schedules.ResetSchedules();
    }

    pimpl_->fulfillment_cache.clear();
    pimpl_->num_child_stages = 0;
}

CachedPipelineInterface *PipelineStage::WithPipeline(PipelineKey const &key,
                                                     CompilePipelineFn compile_fn) {
    auto it = pimpl_->pipelines.find(key);
    if (it != pimpl_->pipelines.end()) {
        return it->second.pipeline.get();
    }

    std::unique_ptr<CachedPipelineInterface> pipeline = compile_fn(pimpl_->output.get());
    it = pimpl_->pipelines.insert(std::make_pair(key, PipelineAndSchedules(std::move(pipeline))))
             .first;

    return it->second.pipeline.get();
}

void PipelineStage::Schedule(CachedPipelineInterface const *pipeline,
                             std::unique_ptr<CachedPipelineArgumentsInterface> &&args,
                             std::vector<PipelineStage *> const &parents) {
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

PipelineOutputInterface const *PipelineStage::Output() const { return pimpl_->output.get(); }

PipelineOutputInterface *PipelineStage::Output() { return pimpl_->output.get(); }

void PipelineStage::Fulfill(VulkanContext *context) {
    this->LaunchSchedulesAs(/*child_id=*/std::nullopt);

    for (auto parent : this->UniqueParents()) {
        WaitForStageCompletion(parent->pimpl_->fulfillment_cache, context);
    }

    // Resets states for taking new schedules.
    this->Reset();
}

} // namespace e8
