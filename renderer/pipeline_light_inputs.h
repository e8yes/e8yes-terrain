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

#ifndef ISLANDS_RENDERER_PIPELINE_GEOMETRY_MAP_H
#define ISLANDS_RENDERER_PIPELINE_GEOMETRY_MAP_H

#include <vector>

#include "common/device.h"
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_output.h"
#include "renderer/projection.h"
#include "renderer/vram_geometry.h"

namespace e8 {

/**
 * @brief The LightInputsPipeline class
 */
class LightInputsPipeline {
  public:
    /**
     * @brief LightInputsPipeline
     * @param output
     * @param context
     */
    LightInputsPipeline(PipelineOutputInterface *output, VulkanContext *context);
    ~LightInputsPipeline();

    /**
     * @brief Run
     *
     * @param drawables
     * @param projection
     * @param barrier
     * @param geo_vram
     * @return
     */
    PipelineOutputInterface *Run(std::vector<DrawableInstance> const &drawables,
                                 ProjectionInterface const &projection, GpuBarrier const &barrier,
                                 GeometryVramTransfer *geo_vram);

  private:
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_GEOMETRY_MAP_H
