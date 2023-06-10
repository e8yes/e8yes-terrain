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

#ifndef ISLANDS_RENDERER_LIGHT_SOURCE_H
#define ISLANDS_RENDERER_LIGHT_SOURCE_H

#include <optional>
#include <vector>

#include "content/proto/light_source.pb.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include <common/tensor.h>

namespace e8 {

/**
 * @brief The LightSourceLod enum
 */
enum LightSourceLod {
    LSL_LEVEL0,
    LSL_LEVEL1,
    LSL_LEVEL2,
    LSL_LEVEL3,
    LSL_LEVEL4,
};

/**
 * @brief The SunLightVolume struct
 */
struct SunLightVolume {
    // Details level that should be used on the sun light region.
    LightSourceLod lod;

    // The region which encloses a part of the sun light's direct illumination.
    aabb b;
};

/**
 * @brief The PointLightVolume struct
 */
struct PointLightVolume {
    // Details level that should be used on the point light.
    LightSourceLod lod;

    // The region which encloses the point light's direct illumination.
    aabb b;
};

/**
 * @brief The SpotLightVolume struct A weighted projection frustum which enclose a spot light's
 * direct illumination
 */
struct SpotLightVolume {
    SpotLightVolume(LightSourceLod lod, PerspectiveProjection projection);

    // Details level that should be used on the spot light.
    LightSourceLod lod;

    // A projection frustum which encloses the spot light's direct illumination.
    PerspectiveProjection projection;
};

/**
 * @brief The LightVolume struct Weighted geometries which enclose the space illuminated by one of
 * the light sources.
 */
struct LightVolume {
    // Filled when the light source is a sun light. Theoretically, a sun light can shine on a
    // infinite region. However, these region LODs are generated according to the camera view.
    std::vector<SunLightVolume> sun_light_regions;

    // Filled when the light source is a point light.
    std::optional<PointLightVolume> point_light_region;

    // Filled when the light source is a spot light.
    std::optional<SpotLightVolume> spot_light_region;
};

/**
 * @brief The LightSourceInstance struct An instance of light source which contains the light source
 * parameters and metadata.
 */
struct LightSourceInstance {
    // Static unique ID assigned to this light source.
    SceneEntityId id;

    // A light source defined in the view space.
    LightSource light_source;

    // Weighted geometries which enclose the space directly illuminated by the light source.
    LightVolume light_volume;
};

/**
 * @brief ToLightSources Extracts light sources from the scene entity list. It attaches additional
 * information about each light source and transforms them into the view space.
 */
std::vector<LightSourceInstance>
ToLightSources(std::vector<SceneEntity const *> const &scene_entities,
               ProjectionInterface const &camera_projection);

} // namespace e8

#endif // ISLANDS_RENDERER_LIGHT_SOURCE_H
