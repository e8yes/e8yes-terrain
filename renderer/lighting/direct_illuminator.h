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

#ifndef ISLANDS_RENDERER_DIRECT_ILLUMINATOR_H
#define ISLANDS_RENDERER_DIRECT_ILLUMINATOR_H

#include <memory>
#include <vector>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The DirectIlluminator class Computes the direct illumination from a list of light sources
 * on a map of lighting parameters.
 */
class DirectIlluminator {
  public:
    DirectIlluminator(DescriptorSetAllocator *desc_set_alloc, VulkanContext *context);
    ~DirectIlluminator();

    /**
     * @brief Run Computes the direct illumination from a list of light sources
     * on a map of lighting parameters.
     *
     * @param light_sources Light sources to compute direct illumination for.
     * @param parameter_map Lighting parameters mapped to the screen.
     * @param parameter_projection The perspective projection setup which generated the parameter
     * map.
     * @param radiance_map Direct illumination result. Note, the radiance map is cleared to black
     * prior to the direct illumination computation.
     */
    void Run(std::vector<LightSourceInstance> const &light_sources,
             LightInputsPipelineOutput const &parameter_map,
             PerspectiveProjection const &parameter_projection,
             HdrColorPipelineOutput *radiance_map);

  private:
    struct DirectIlluminatorImpl;

    std::unique_ptr<DirectIlluminatorImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DIRECT_ILLUMINATOR_H
