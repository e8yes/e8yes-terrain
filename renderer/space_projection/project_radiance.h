/**
 * e8yes islands.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

#ifndef ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_RADIANCE_H
#define ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_RADIANCE_H

#include <memory>
#include <optional>
#include <vector>

#include "common/tensor.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/drawable/collection.h"
#include "renderer/drawable/light_source.h"

namespace e8 {

/**
 * @brief The ShadowedSunLight class
 */
struct ShadowedSunLight {
    ShadowedSunLight(LightSourceInstance const &sun_light,
                     std::vector<DagOperationInstance> const &cascaded_shadow_maps);

    LightSourceInstance sun_light;
    std::vector<DagOperationInstance> cascaded_shadow_maps;
};

/**
 * @brief The ShadowedSpotLight class
 */
struct ShadowedSpotLight {
    ShadowedSpotLight(LightSourceInstance const &spot_light,
                      DagOperationInstance const &shadow_map);

    LightSourceInstance spot_light;
    DagOperationInstance shadow_map;
};

/**
 * @brief DoProjectRadiance Schedules a graphics pipeline to generate a map of radiance value.
 *
 * @param projected_surface
 * @param projection
 * @param shadowed_sunlight
 * @param shadowed_spot_lights
 * @param spot_lights
 * @param point_lights
 * @param session The DAG session.
 * @return The operation which computes the radiance map as an HDR color image.
 */
DagOperationInstance DoProjectRadiance(std::vector<DrawableInstance> const &drawables,
                                       std::optional<ShadowedSunLight> const &shadowed_sun_light,
                                       std::vector<ShadowedSpotLight> const &shadowed_spot_lights,
                                       std::vector<LightSourceInstance> const &spot_lights,
                                       std::vector<LightSourceInstance> const &point_lights,
                                       PerspectiveProjection const &projection,
                                       DagOperationInstance projected_surface,
                                       DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_RADIANCE_H
