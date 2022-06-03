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

#ifndef ISLANDS_RENDERER_PIPELINE_STAGE_H
#define ISLANDS_RENDERER_PIPELINE_STAGE_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/device.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"

namespace e8 {

/**
 * @brief The PipelineStage class Encapsulates a set of mudular independent operations that can be
 * chained into a DAG compute graph.
 */
class PipelineStage {
  public:
    // The key of a cached pipeline.
    using PipelineKey = std::string;

    // A graphics pipeline's construction is often paired with an output. This function standarized
    // the creation of a complete graphics pipeline.
    using CompilePipelineFn =
        std::function<std::unique_ptr<CachedPipelineInterface>(PipelineOutputInterface *output)>;

    /**
     * @brief PipelineStage Constructs a pipeline stage attached with an optional output.
     */
    PipelineStage(std::shared_ptr<PipelineOutputInterface> const &output);
    PipelineStage(PipelineStage const &) = delete;
    ~PipelineStage();

    /**
     * @brief WithPipeline Adds a graphics pipeline to this stage if it hasn't been.
     *
     * @param key A unique key which identifies the pipeline.
     * @param compile_fn The pipeline's constructor.
     * @return The graphics pipeline associated with the key.
     */
    CachedPipelineInterface *WithPipeline(PipelineKey const &key, CompilePipelineFn compile_fn);

    /**
     * @brief Schedule Schedules an added pipeline to be run with the specified parameters. The
     * pipeline is guaranteed to have run after calling PipelineStage::Fulfill().
     *
     * @param key A unique key which identifies the pipeline.
     * @param parents The stages where the pipeline depends.
     * @param instance_count The number of times to call on the pipeline with the same dependent
     * parents.
     */
    void Schedule(PipelineKey const &key, std::vector<PipelineStage *> const &parents,
                  unsigned instance_count);

    /**
     * @brief Output The output attached to this stage.
     */
    PipelineOutputInterface *Output();

    /**
     * @brief Fulfill Runs all schedules in this stage and those in the dependent parents, then it
     * waits for the parents to resolve (complete). Note, it doesn't wait for the completion of this
     * stage because, typically, the final stage is a v-sync that has to be resolved in the next
     * frame.
     *
     * @param context Contextual Vulkan handles.
     */
    void Fulfill(VulkanContext *context);

  private:
    using ChildId = unsigned;

    struct PipelineStageImpl;
    struct PipelineAndSchedule;

    ChildId AllocateChild();

    std::unordered_set<PipelineStage *> Parents();

    void
    LaunchSchedule(PipelineKey const &pipeline_key,
                   PipelineAndSchedule const &pipeline_and_schedule, unsigned child_count,
                   PipelineOutputInterface *output,
                   std::unordered_map<PipelineKey, std::vector<Fulfillment>> *fulfillment_cache);

    std::vector<GpuPromise *> LaunchSchedulesFor(std::optional<ChildId> child_id);

    void Reset();

    std::unique_ptr<PipelineStageImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_STAGE_H
