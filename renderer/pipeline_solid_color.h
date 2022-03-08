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

#ifndef ISLANDS_RENDERER_PIPELINE_SOLID_COLOR_H
#define ISLANDS_RENDERER_PIPELINE_SOLID_COLOR_H

#include <memory>

#include "common/tensor.h"
#include "renderer/context.h"
#include "renderer/pipeline_output.h"

namespace e8 {

/**
 * @brief The SolidColorPipeline class A graphics pipeline for filling the color output with a solid
 * color.
 */
class SolidColorPipeline {
  public:
    /**
     * @brief SolidColorPipeline Constructs a graphics pipeline for filling the color output with a
     * solid color.
     *
     * @param output The output to be filled with solid color.
     * @param context Contextual Vulkan handles.
     */
    SolidColorPipeline(PipelineOutputInterface *output, VulkanContext *context);
    ~SolidColorPipeline();

    /**
     * @brief Run Runs the solid color graphics pipeline. The pipeline can only be run when the
     * previous run was finished (indicated by the output's barrier).
     *
     * @param color The color value to fill into the output.
     * @param barrier The previous tasks' barrier.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(vec3 const &color, GpuBarrier const &barrier);

  private:
    struct SolidColorPipelineImpl;
    std::unique_ptr<SolidColorPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_SOLID_COLOR_H