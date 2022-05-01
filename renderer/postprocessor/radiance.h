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

#ifndef ISLANDS_RENDERER_RADIANCE_PIPELINE_H
#define ISLANDS_RENDERER_RADIANCE_PIPELINE_H

#include <memory>

#include "common/device.h"
#include "common/tensor.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The RadiancePipeline class Computes the direct radiance produced by a light source for
 * each pixel.
 */
class RadiancePipeline {
  public:
    RadiancePipeline(DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~RadiancePipeline();

    /**
     * @brief Run Computes the radiance produced by the specified light source based on screen-space
     * geometric and material parameters. Note, the light source must be defined in the view space
     * where the light inputs were generated.
     */
    void Run(LightSourceInstance const &instance, LightInputsPipelineOutput const &light_inputs,
             frustum const &light_inputs_frustum, GpuPromise const &promise,
             HdrColorPipelineOutput *output);

  private:
    struct DirectionalRadiancePipelineImpl;

    DescriptorSetAllocator *desc_set_allocator_;
    VulkanContext *context_;
    HdrColorPipelineOutput *current_output_;
    std::unique_ptr<DirectionalRadiancePipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_RADIANCE_PIPELINE_H
