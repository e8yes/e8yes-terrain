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

#ifndef PIPELINE_DEPTH_MAP_VISUALIZER_H
#define PIPELINE_DEPTH_MAP_VISUALIZER_H

#include <memory>

#include "renderer/context.h"
#include "renderer/pipeline_output.h"

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
     * @param context Contextual Vulkan handles.
     */
    DepthMapVisualizerPipeline(PipelineOutputInterface *visualizer_output, VulkanContext *context);

    ~DepthMapVisualizerPipeline();

    /**
     * @brief Run Runs the depth map visualizer graphics pipeline. The pipeline can only be run when
     * the previous run was finished (indicated by the output's barrier).
     *
     * @param depth_map The depth map to be visualized.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(PipelineOutputInterface const &depth_map);

  private:
    struct DepthMapVisualizerPipelineImpl;
    std::unique_ptr<DepthMapVisualizerPipelineImpl> pimpl_;
};

} // namespace e8

#endif // PIPELINE_DEPTH_MAP_VISUALIZER_H
