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

#ifndef ISLANDS_RENDERER_PIPELINE_DEPTH_MAP_H
#define ISLANDS_RENDERER_PIPELINE_DEPTH_MAP_H

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/tensor.h"
#include "renderer/context.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_output.h"
#include "renderer/projection.h"
#include "renderer/render_pass.h"
#include "renderer/vram_geometry.h"

namespace e8 {

/**
 * @brief The DepthMapPipeline class A graphics pipeline for rendering a depth map.
 */
class DepthMapPipeline {
  public:
    /**
     * @brief DepthMapPipeline Constructs a graphics pipeline for rendering a depth map.
     *
     * @param output An object for storing the rendered depth map.
     * @param context Contextual Vulkan handles.
     */
    DepthMapPipeline(PipelineOutputInterface *output, VulkanContext *context);
    ~DepthMapPipeline();

    /**
     * @brief Run Runs the depth map graphics pipeline. The pipeline can only be run when the
     * previous run was finished (indicated by the output's barrier).
     *
     * @param drawables An array of drawables to be rendered onto the depth map.
     * @param projection Defines how drawables should be projected to the depth map.
     * @param barrier The previous tasks' barrier.
     * @param geo_vram The geometry VRAM transferrer.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(std::vector<DrawableInstance> const &drawables,
                                 ProjectionInterface const &projection, GpuBarrier const &barrier,
                                 GeometryVramTransfer *geo_vram);

  private:
    class DepthMapPipelineImpl;

    std::unique_ptr<DepthMapPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_DEPTH_MAP_H
