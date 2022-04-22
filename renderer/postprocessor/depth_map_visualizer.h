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

#ifndef DEPTH_MAP_VISUALIZER_H
#define DEPTH_MAP_VISUALIZER_H

#include <memory>
#include <optional>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/pipeline/depth_map.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The DepthMapVisualizerPipeline class A post processing graphics pipeline for
 * visualizing a rendered depth map.
 */
class DepthMapVisualizerPipeline {
  public:
    /**
     * @brief DepthMapVisualizerPipeline Constructs a post processing graphics pipeline for
     * visualizing a rendered depth map.
     *
     * @param visualizer_output An object for storing the depth map image in grayscale.
     * @param desc_set_allocator
     * @param context Contextual Vulkan handles.
     */
    DepthMapVisualizerPipeline(DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);

    ~DepthMapVisualizerPipeline();

    /**
     * @brief Run Runs the depth map visualizer graphics pipeline. The pipeline can only be run when
     * the previous run was finished (indicated by the output's barrier).
     *
     * @param alpha When alpha is zero, the depth map is visualized using the raw NDC value. When
     * it is set to one, the value is corrected to display the true depth.
     * @param projection The projection used to create the depth map. If alpha is non-zero, this
     * argument is required in order to get the correct result.
     * @param depth_map The depth map to be visualized.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    void Run(float alpha, std::optional<PerspectiveProjection> projection,
             DepthMapPipelineOutput const &depth_map, PipelineOutputInterface *visualizer_output);

  private:
    struct DepthMapVisualizerPipelineImpl;
    DescriptorSetAllocator *desc_set_allocator_;
    VulkanContext *context_;
    PipelineOutputInterface *current_output_;
    std::unique_ptr<DepthMapVisualizerPipelineImpl> pimpl_;
};

} // namespace e8

#endif // DEPTH_MAP_VISUALIZER_H
