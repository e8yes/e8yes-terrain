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
#include <unordered_set>
#include <vector>

#include "common/device.h"
#include "content/proto/light_source.pb.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include "renderer/lighting/direct_illuminator.h"
#include "renderer/output/common_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/pipeline/fill_color.h"
#include "renderer/pipeline/project_depth.h"
#include "renderer/postprocessor/gaussian_blur.h"
#include "renderer/postprocessor/radiance.h"
#include "renderer/query/light_source.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

unsigned kSpotLightShadowMapWidth = 512;
unsigned kSpotLightShadowMapHeight = 512;
unsigned kSpotLightBlurringKernelSize = 3;

struct ShadowMapCollection {
    std::vector<std::unique_ptr<PipelineStage>> projected_ndc_depth;
    std::vector<std::unique_ptr<PipelineStage>> projected_linear_depth;
    std::vector<std::unique_ptr<PipelineStage>> h_blurred;
    std::vector<std::unique_ptr<PipelineStage>> hv_blurred;
};

class ShadowMapCache {
  public:
    ShadowMapCache();
    ~ShadowMapCache();

    void EvictIfNotObservable(std::vector<LightSourceInstance> const &observable_light_sources);
    ShadowMapCollection *Allocate(LightSourceInstance const &light_source);
    std::vector<PipelineStage *> Fetch(SceneEntityId light_id) const;

  private:
    std::unordered_map<SceneEntityId, std::unique_ptr<ShadowMapCollection>> shadow_maps_;
};

ShadowMapCache::ShadowMapCache() {}

ShadowMapCache::~ShadowMapCache() {}

void ShadowMapCache::EvictIfNotObservable(
    std::vector<LightSourceInstance> const &observable_light_sources) {
    std::unordered_set<SceneEntityId> observable_set;
    for (auto const &light_source : observable_light_sources) {
        observable_set.insert(light_source.id);
    }

    std::vector<SceneEntityId> to_be_evicted;
    for (auto const &[light_id, _] : shadow_maps_) {
        if (observable_set.find(light_id) == observable_set.end()) {
            to_be_evicted.push_back(light_id);
        }
    }

    for (auto light_id : to_be_evicted) {
        shadow_maps_.erase(light_id);
    }
}

ShadowMapCollection *ShadowMapCache::Allocate(LightSourceInstance const &light_source) {
    // TODO: Allocates shadow map stages and chooses shadow map resolution based on LightSourceLod
    // level.

    auto it = shadow_maps_.insert(
        std::make_pair(light_source.id, std::make_unique<ShadowMapCollection>()));
    auto &[_, shadow_map_collection] = *it.first;
    return shadow_map_collection.get();
}

std::vector<PipelineStage *> ShadowMapCache::Fetch(SceneEntityId light_id) const {
    auto it = shadow_maps_.find(light_id);
    if (it == shadow_maps_.end()) {
        return std::vector<PipelineStage *>();
    }

    auto const &[_, shadow_map_collection] = *it;

    std::vector<PipelineStage *> result(shadow_map_collection->hv_blurred.size());
    for (unsigned i = 0; i < shadow_map_collection->hv_blurred.size(); ++i) {
        result[i] = shadow_map_collection->hv_blurred[i].get();
    }

    return result;
}

void DoGenerateSpotLightShadowMap(SpotLightVolume const &spot_light_region,
                                  DrawableCollection *drawable_collection,
                                  PipelineStage *first_stage, TransferContext *transfer_context,
                                  ShadowMapCollection *shadow_maps) {
    shadow_maps->projected_ndc_depth.clear();
    shadow_maps->projected_linear_depth.clear();
    shadow_maps->h_blurred.clear();
    shadow_maps->hv_blurred.clear();

    std::unique_ptr<PipelineStage> projected_ndc_depth;
    std::unique_ptr<PipelineStage> projected_linear_depth;
    projected_ndc_depth = CreateProjectDepthStage(
        kSpotLightShadowMapWidth, kSpotLightShadowMapHeight, transfer_context->vulkan_context);
    projected_linear_depth = CreateProjectDepthStage(
        kSpotLightShadowMapWidth, kSpotLightShadowMapHeight, transfer_context->vulkan_context);

    std::unique_ptr<PipelineStage> h_blurred;
    std::unique_ptr<PipelineStage> hv_blurred;
    CreateGaussianBlurStages(kSpotLightShadowMapWidth, kSpotLightShadowMapHeight,
                             transfer_context->vulkan_context, &h_blurred, &hv_blurred);

    DoProjectDepth(drawable_collection, spot_light_region.projection, transfer_context, first_stage,
                   projected_ndc_depth.get(), projected_linear_depth.get());
    DoGaussianBlur(projected_linear_depth.get(), kSpotLightBlurringKernelSize, transfer_context,
                   h_blurred.get(), hv_blurred.get());

    shadow_maps->projected_ndc_depth.emplace_back(std::move(projected_ndc_depth));
    shadow_maps->projected_linear_depth.emplace_back(std::move(projected_linear_depth));
    shadow_maps->h_blurred.emplace_back(std::move(h_blurred));
    shadow_maps->hv_blurred.emplace_back(std::move(hv_blurred));
}

void DoGenerateShadowMaps(std::vector<LightSourceInstance> const &light_sources,
                          DrawableCollection *drawable_collection, PipelineStage *first_stage,
                          TransferContext *transfer_context, ShadowMapCache *cache) {
    cache->EvictIfNotObservable(light_sources);

    for (auto const &light_source : light_sources) {
        ShadowMapCollection *allocated_shadow_maps = cache->Allocate(light_source);
        if (allocated_shadow_maps == nullptr) {
            continue;
        }

        if (light_source.light_volume.spot_light_region.has_value()) {
            DoGenerateSpotLightShadowMap(*light_source.light_volume.spot_light_region,
                                         drawable_collection, first_stage, transfer_context,
                                         allocated_shadow_maps);
        } else {
            // TODO: Generates shadow map for point lights and sun lights.
        }
    }
}

} // namespace

struct DirectIlluminator::DirectIlluminatorImpl {
    DirectIlluminatorImpl(unsigned width, unsigned height, VulkanContext *context);
    ~DirectIlluminatorImpl();

    std::unique_ptr<PipelineStage> cleared_radiance_map;
    std::unique_ptr<PipelineStage> filled_radiance_map;

    ShadowMapCache shadow_map_cache;
};

DirectIlluminator::DirectIlluminatorImpl::DirectIlluminatorImpl(unsigned width, unsigned height,
                                                                VulkanContext *context) {
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
    DrawableCollection *drawable_collection, PipelineStage *projected_surface,
    PerspectiveProjection const &projection, PipelineStage *first_stage,
    TransferContext *transfer_context) {
    DoFillColor(/*color=*/vec3{0.0f, 0.0f, 0.0f}, transfer_context->vulkan_context, first_stage,
                pimpl_->cleared_radiance_map.get());

    std::vector<LightSourceInstance> light_sources =
        drawable_collection->ObservableLightSources(projection);
    if (light_sources.empty()) {
        return pimpl_->cleared_radiance_map.get();
    }

    DoGenerateShadowMaps(light_sources, drawable_collection, first_stage, transfer_context,
                         &pimpl_->shadow_map_cache);

    for (auto const &instance : light_sources) {
        std::vector<PipelineStage *> shadow_maps = pimpl_->shadow_map_cache.Fetch(instance.id);

        DoComputeRadiance(instance, projected_surface, projection.Frustum(), shadow_maps,
                          pimpl_->cleared_radiance_map.get(), transfer_context,
                          pimpl_->filled_radiance_map.get());
    }

    return pimpl_->filled_radiance_map.get();
}

} // namespace e8
