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

#ifndef ISLANDS_RENDERER_DAG_CONTEXT_H
#define ISLANDS_RENDERER_DAG_CONTEXT_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "common/device.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief The DagContext class It acts as a registrar for all DAG operations in a compute graph, so
 * the renderers don't have to maintain them separately. It also help track their usage and evict
 * those that are used infrequently (TODO) from cache memory. This class isn't thread-safe but
 * thread-compatible.
 */
class DagContext {
  public:
    /**
     * @brief DagContext Constructs a DAG graph under the specified Vulkan context.
     */
    DagContext(VulkanContext *context);
    ~DagContext();

    using CreateDagOperationFn = std::function<std::unique_ptr<DagOperation>(
        unsigned output_width, unsigned output_height, TransferContext *transfer_context,
        VulkanContext *vulkan_context)>;

    class Session {
      public:
        Session(DagContext *dag_context);
        Session(Session const &) = delete;
        Session(Session &&other);
        ~Session();

        /**
         * @brief WithOperation Adds a DagOperation to the inherited context if it hasn't been. The
         * operation is identified through the tuple (graphics_pipeline_key, output_width,
         * output_height, sessional_allocation_counter). This means effectively the same DAG
         * operation of the same pipeline and output size will be fetched in the next session,
         * avoiding re-creation. This identification method also allows multiple operations with the
         * same format to be created within a session.
         *
         * @param pipeline_key A unique key which identifies the DagOperation.
         * @param output_width The output width of the pipeline.
         * @param output_height The output height of the pipeline.
         * @param create_fn The DagOperation's constructor.
         * @return The DagOperation associated with the key. Always not none.
         */
        DagOperationInstance WithOperation(PipelineKey const &keypipeline_key,
                                           unsigned output_width, unsigned output_height,
                                           CreateDagOperationFn create_fn);

      private:
        DagContext *self_;
    };

    /**
     * @brief CreateSession Only one session can be created at a time for every DagContext.
     */
    Session CreateSession();

  private:
    using SessionalAllocationCounter = unsigned;
    using DagOperationKeyPrefix = std::string;
    using DagOperationKey = std::string;

    struct DagOperationWithUsage {
        std::unique_ptr<DagOperation> dag_op;
        uint64_t usage_count = 0;
    };

    static DagOperationKeyPrefix CreateKeyPrefix(PipelineKey const &pipeline_key,
                                                 unsigned output_width, unsigned output_height);
    DagOperationKey AllocateSessionalKey(DagOperationKeyPrefix const &key_prefix);
    DagOperationInstance FetchDagOperation(DagOperationKey const &sessional_op_key,
                                           unsigned output_width, unsigned output_height,
                                           CreateDagOperationFn create_fn);
    void ResetKeyAllocation();
    void DeleteUnusedDagOperations();

    VulkanContext *vulkan_context_;
    TransferContext transfer_context_;

    std::unordered_map<DagOperationKey, DagOperationWithUsage> dag_ops_;
    std::unordered_map<DagOperationKeyPrefix, SessionalAllocationCounter> sess_alloc_counters_;

    uint64_t session_count_;
    bool in_session_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DAG_CONTEXT_H
