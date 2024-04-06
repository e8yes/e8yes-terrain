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

#include <cassert>
#include <memory>
#include <unordered_set>
#include <vector>

#include "content/proto/light_source.pb.h"
#include "renderer/basic/projection.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/drawable/light_source.h"
#include "renderer/space_projection/project_depth.h"
#include "renderer/space_screen/fill_color.h"
#include "renderer/space_screen/gaussian_blur.h"
#include "renderer/space_screen/radiance.h"

namespace e8 {
namespace {

unsigned kSpotLightShadowMapWidth = 512;
unsigned kSpotLightShadowMapHeight = 512;
GaussianBlurLevel kSpotLightBlurringKernelSize = GaussianBlurLevel::GBL_1X1;

DagOperationInstance DoGenerateSpotLightShadowMap(SpotLightVolume const &spot_light_region,
                                                  DrawableCollection *drawable_collection,
                                                  DagContext::Session *session) {
    DagOperationInstance projected_linear_depth = DoProjectLinearDepth(
        drawable_collection, spot_light_region.projection, kSpotLightShadowMapWidth,
        kSpotLightShadowMapHeight, /*dependent_op=*/nullptr, session);
    return projected_linear_depth;
}

std::vector<DagOperationInstance> DoGenerateShadowMaps(LightSourceInstance const &light_source,
                                                       DrawableCollection *drawable_collection,
                                                       DagContext::Session *session) {

    if (light_source.light_volume.spot_light_region.has_value()) {
        DagOperationInstance shadow_map = DoGenerateSpotLightShadowMap(
            *light_source.light_volume.spot_light_region, drawable_collection, session);
        // TODO: Blur the shadow map with kSpotLightBlurringKernelSize.
        return std::vector<DagOperationInstance>({shadow_map});
    } else {
        // TODO: Generates shadow map for point lights and sun lights.
        return std::vector<DagOperationInstance>();
    }
}

} // namespace

DagOperationInstance DoComputeDirectIllumination(DrawableCollection *drawable_collection,
                                                 DagOperationInstance projected_surface,
                                                 PerspectiveProjection const &projection,
                                                 DagContext::Session *session) {

    std::vector<LightSourceInstance> light_sources =
        drawable_collection->ObservableLightSources(projection);
    if (light_sources.empty()) {
        return DoFillColor(/*color=*/vec3{0.0f, 0.0f, 0.0f}, /*hdr=*/true, projected_surface,
                           session);
    }

    std::optional<LightSourceInstance> sun_light;
    std::vector<LightSourceInstance> spot_lights;
    std::vector<LightSourceInstance> point_lights;

    for (auto const &light : light_sources) {
        switch (light.light_source.model_case()) {
        case LightSource::ModelCase::kSunLight: {
            *sun_light = light;
            break;
        }
        case LightSource::ModelCase::kPointLight: {
            point_lights.push_back(light);
            break;
        }
        case LightSource::ModelCase::kSpotLight: {
            spot_lights.push_back(light);
            break;
        }
        default: {
            assert(false);
        }
        }
    }

    return DoComputeRadiance(projected_surface, projection.Frustum(),
                             /*shadowed_sunlight=*/std::nullopt,
                             /*shadowed_spot_lights=*/std::vector<ShadowedSpotLight>(), spot_lights,
                             point_lights, session);
}

} // namespace e8
