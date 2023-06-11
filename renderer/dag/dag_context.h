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
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "renderer/dag/dag_operation.h"

namespace e8 {

/**
 * @brief The DagContext class It acts as a registrar for all DAG operations in a compute graph, so
 * the renderers don't have to maintain them separately. It also help track their usage and evict
 * those that are used infrequently (TODO) from cache memory. This class isn't thread-safe but
 * thread-compatible.
 */
class DagContext {
  public:
    DagContext();
    ~DagContext() = default;

    using DagOperationKey = std::string;
    using CreateDagOperationFn = std::function<std::unique_ptr<DagOperation>()>;

    class Session {
      public:
        Session(DagContext *dag_context);
        Session(Session const &) = delete;
        Session(Session &&other);
        ~Session();

      private:
        DagContext *self_;
    };

    /**
     * @brief CreateSession Only one session can be created at a time for every DagContext.
     */
    Session CreateSession();

    /**
     * @brief WithOperation Adds a DagOperation to this context if it hasn't been. This function can
     * only be called in a session (See CreateSession() above).
     *
     * @param key A unique key which identifies the DagOperation.
     * @param create_fn The DagOperation's constructor.
     * @return The DagOperation associated with the key. Always not none.
     */
    DagOperation *WithOperation(const DagOperationKey &key, CreateDagOperationFn create_fn);

  private:
    struct DagOperationWithUsage {
        std::unique_ptr<DagOperation> dag_op;
        uint64_t usage_count = 0;
    };

    std::unordered_map<DagOperationKey, DagOperationWithUsage> dag_ops_;
    uint64_t session_count_;
    bool in_session_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DAG_CONTEXT_H
