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
#include <unordered_map>
#include <vector>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/lighting/direct_illuminator.h"
#include "renderer/output/common_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/pipeline/depth_map.h"
#include "renderer/pipeline/fill_color.h"
#include "renderer/postprocessor/radiance.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {
namespace {

unsigned kSpotLightShadowMapWidth = 512;
unsigned kSpotLightShadowMapHeight = 512;
unsigned kMaxSpotLightShadowMaps = 1;

struct ShadowMapCache {
    ShadowMapCache(VulkanContext *context);
    ~ShadowMapCache();

    std::vector<std::unique_ptr<PipelineStage>> spot_light_shadow_maps;
};

ShadowMapCache::ShadowMapCache(VulkanContext *context) {
    for (unsigned i = 0; i < kMaxSpotLightShadowMaps; ++i) {
        std::unique_ptr<PipelineStage> spot_light_shadow_map =
            CreateDepthMapStage(kSpotLightShadowMapWidth, kSpotLightShadowMapHeight, context);
        spot_light_shadow_maps.push_back(std::move(spot_light_shadow_map));
    }
}

ShadowMapCache::~ShadowMapCache() {}

void DoGenerateShadowMaps(
    std::vector<LightSourceInstance> const &light_sources, VulkanContext *context,
    std::unordered_map<LightSourceInstance const *, std::vector<PipelineStage *>>
        *light_source_shadow_maps,
    ShadowMapCache *cache) {}

}  // namespace

struct DirectIlluminator::DirectIlluminatorImpl {
    DirectIlluminatorImpl(unsigned width, unsigned height, VulkanContext *context);
    ~DirectIlluminatorImpl();

    VulkanContext *context;

    std::unique_ptr<PipelineStage> cleared_radiance_map;
    std::unique_ptr<PipelineStage> filled_radiance_map;

    ShadowMapCache shadow_map_cache;
};

DirectIlluminator::DirectIlluminatorImpl::DirectIlluminatorImpl(unsigned width, unsigned height,
                                                                VulkanContext *context)
    : context(context), shadow_map_cache(context) {
    auto output = std::make_shared<HdrColorPipelineOutput>(width, height,
                                                           /*with_depth_buffer=*/false, context);
    cleared_radiance_map = std::make_unique<PipelineStage>(output);
    filled_radiance_map = std::make_unique<PipelineStage>(output);
}

DirectIlluminator::DirectIlluminatorImpl::~DirectIlluminatorImpl() {}

DirectIlluminator::DirectIlluminator(unsigned width, unsigned height, VulkanContext *context)
    : pimpl_(std::make_unique<DirectIlluminatorImpl>(width, height, context)) {}

DirectIlluminator::~DirectIlluminator() {}

PipelineStage *DirectIlluminator::DoComputeDirectIllumination(
    std::vector<LightSourceInstance> const &light_sources,
    PerspectiveProjection const &parameter_projection, PipelineStage *parameter_map,
    PipelineStage *first_stage, DescriptorSetAllocator *desc_set_alloc) {
    DoFillColor(/*color=*/vec3{0.0f, 0.0f, 0.0f}, pimpl_->context, first_stage,
                pimpl_->cleared_radiance_map.get());

    if (light_sources.empty()) {
        return pimpl_->cleared_radiance_map.get();
    }

    std::unordered_map<LightSourceInstance const *, std::vector<PipelineStage *>>
        light_source_shadow_maps;
    DoGenerateShadowMaps(light_sources, pimpl_->context, &light_source_shadow_maps,
                         &pimpl_->shadow_map_cache);

    for (auto const &instance : light_sources) {
        std::vector<PipelineStage *> shadow_maps;
        auto it = light_source_shadow_maps.find(&instance);
        if (it != light_source_shadow_maps.end()) {
            shadow_maps = it->second;
        }

        DoComputeRadiance(instance, parameter_projection.Frustum(), parameter_map,
                          /*shadow_map=*/shadow_maps, pimpl_->cleared_radiance_map.get(),
                          desc_set_alloc, pimpl_->context, pimpl_->filled_radiance_map.get());
    }

    return pimpl_->filled_radiance_map.get();
}

}  // namespace e8
