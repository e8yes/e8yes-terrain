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

#ifndef ISLANDS_RENDERER_RADIANCE_DIRECTIONAL_H
#define ISLANDS_RENDERER_RADIANCE_DIRECTIONAL_H

#include <memory>

#include "common/device.h"
#include "content/proto/light_source.pb.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The DirectionalRadiancePipeline class Computes the direct radiance produced by a
 * directional light source for each pixel.
 */
class DirectionalRadiancePipeline {
  public:
    DirectionalRadiancePipeline(UnboundedColorPipelineOutput *radiance_output,
                                DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~DirectionalRadiancePipeline();

    /**
     * @brief Run Computes the radiance produced by the specified sun light based on screen-space
     * geometric and material parameters.
     */
    UnboundedColorPipelineOutput *Run(SunLight const &light, ProjectionInterface const &projection,
                                      LightInputsPipelineOutput const &light_inputs);

  private:
    struct DirectionalRadiancePipelineImpl;
    std::unique_ptr<DirectionalRadiancePipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_RADIANCE_DIRECTIONAL_H
