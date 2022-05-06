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

#include <memory>
#include <vector>

#include "common/device.h"
#include "renderer/lighting/direct_illuminator.h"
#include "renderer/output/common_output.h"
#include "renderer/pipeline/solid_color.h"
#include "renderer/postprocessor/radiance.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

struct DirectIlluminator::DirectIlluminatorImpl {
    DirectIlluminatorImpl(DescriptorSetAllocator *desc_set_alloc, VulkanContext *context);
    ~DirectIlluminatorImpl();

    SolidColorPipeline solid_color_pipeline;
    RadiancePipeline radiance_pipeline;
};

DirectIlluminator::DirectIlluminatorImpl::DirectIlluminatorImpl(
    DescriptorSetAllocator *desc_set_alloc, VulkanContext *context)
    : solid_color_pipeline(context), radiance_pipeline(desc_set_alloc, context) {}

DirectIlluminator::DirectIlluminatorImpl::~DirectIlluminatorImpl() {}

DirectIlluminator::DirectIlluminator(DescriptorSetAllocator *desc_set_alloc, VulkanContext *context)
    : pimpl_(std::make_unique<DirectIlluminatorImpl>(desc_set_alloc, context)) {}

DirectIlluminator::~DirectIlluminator() {}

void DirectIlluminator::Run(std::vector<LightSourceInstance> const &light_sources,
                            LightInputsPipelineOutput const &parameter_map,
                            PerspectiveProjection const &parameter_projection,
                            HdrColorPipelineOutput *radiance_map) {
    pimpl_->solid_color_pipeline.Run(vec3{0.0f, 0.0f, 0.0f}, *parameter_map.Promise(),
                                     radiance_map);

    for (auto instance : light_sources) {
        pimpl_->radiance_pipeline.Run(instance, parameter_map, parameter_projection.Frustum(),
                                      *radiance_map->Promise(), radiance_map);
    }
}

} // namespace e8
