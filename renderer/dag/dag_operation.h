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

#ifndef ISLANDS_RENDERER_DAG_OPERATION_H
#define ISLANDS_RENDERER_DAG_OPERATION_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/device.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/promise.h"

namespace e8 {

/**
 * @brief The DagOperation class Each DAG operation is a node in a DAG compute graph. An operation
 * serves as a container for a series of independent graphics pipelines. During fulfillment, all
 * graphics pipelines are launched before moving on to child operations.
 */
class DagOperation {
  public:
    // A graphics pipeline's construction is often paired with an output. This function standarized
    // the creation of a complete graphics pipeline.
    using CompilePipelineFn = std::function<std::unique_ptr<GraphicsPipelineInterface>(
        GraphicsPipelineOutputInterface *output)>;

    /**
     * @brief DagOperation Constructs a pipeline stage attached with an optional output.
     */
    explicit DagOperation(std::shared_ptr<GraphicsPipelineOutputInterface> const &output);
    DagOperation(DagOperation const &) = delete;
    ~DagOperation();

    /**
     * @brief WithPipeline Adds a graphics pipeline to this DagOperation if it hasn't been.
     *
     * @param key A unique key which identifies the pipeline.
     * @param compile_fn The pipeline's constructor.
     * @return The graphics pipeline associated with the key.
     */
    GraphicsPipelineInterface *WithPipeline(PipelineKey const &key, CompilePipelineFn compile_fn);

    /**
     * @brief Schedule Schedules an added pipeline to be run with the specified parameters. The
     * pipeline is guaranteed to have run after calling DagOperation::Fulfill().
     *
     * @param pipeline The pipeline to be scheduled to run.
     * @param parents The stages where the pipeline depends. The elements in this array is nullable.
     * In the case when there are nullptrs, this function simply discards them.
     * @param instance_count The number of times to call on the pipeline with the same dependent
     * parents.
     */
    void Schedule(GraphicsPipelineInterface const *pipeline,
                  std::unique_ptr<GraphicsPipelineArgumentsInterface> &&args,
                  std::vector<DagOperation *> const &parents);

    /**
     * @brief Output The immutable output attached to this stage.
     */
    GraphicsPipelineOutputInterface const *Output() const;

    /**
     * @brief Output The output attached to this stage.
     */
    GraphicsPipelineOutputInterface *Output();

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

    struct DagOperationImpl;
    struct ParentStage;
    struct PipelineSchedule;
    struct PipelineAndSchedules;
    ChildId AllocateChild();

    std::unordered_set<DagOperation *> UniqueParents();

    Fulfillment LaunchSchedule(GraphicsPipelineInterface *pipeline,
                               PipelineSchedule const &schedule, unsigned child_count,
                               GraphicsPipelineOutputInterface *output);

    std::vector<GpuPromise *> LaunchSchedulesAs(std::optional<ChildId> child_id);

    void Reset();

    std::unique_ptr<DagOperationImpl> pimpl_;
};

// The most common way to use a DagOperation is through its pointer while the its actual memory sits
// in a global context.
using DagOperationInstance = DagOperation *;

} // namespace e8

#endif // ISLANDS_RENDERER_DAG_OPERATION_H
