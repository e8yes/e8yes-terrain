/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

#ifndef ISLANDS_RENDERER_FRAME_RESOURCE_ALLOCATOR_H
#define ISLANDS_RENDERER_FRAME_RESOURCE_ALLOCATOR_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/dag/promise.h"

namespace e8 {

/**
 * @brief The FrameResourceAllocator class
 */
class FrameResourceAllocator {
  public:
    FrameResourceAllocator(VulkanContext *context);
    ~FrameResourceAllocator();

    /**
     * @brief SetSwapChainImageIndex
     * @param swap_chain_image_index
     */
    void SetSwapChainImageIndex(unsigned swap_chain_image_index);

    /**
     * @brief AllocateCpuPromise
     * @return
     */
    CpuPromise *AllocateCpuPromise();

    /**
     * @brief AllocateGpuPromise
     * @return
     */
    GpuPromise *AllocateGpuPromise();

    /**
     * @brief AllocateCommandBuffer
     * @return
     */
    CommandBuffer *AllocateCommandBuffer();

  private:
    struct InFrameResources {
        std::vector<std::unique_ptr<CpuPromise>> cpu_promises;
        std::vector<std::unique_ptr<GpuPromise>> gpu_promises;
        std::vector<std::unique_ptr<CommandBuffer>> command_buffers;

        unsigned num_cpu_promises_allocated;
        unsigned num_gpu_promises_allocated;
        unsigned num_cmd_buffers_allocated;
    };

    VulkanContext *context_;
    unsigned swap_chain_image_index_;
    std::vector<InFrameResources> frames_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_FRAME_RESOURCE_ALLOCATOR_H
