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

#include <vector>

#include "content/proto/light_source.pb.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"

namespace e8 {

/**
 * @brief The LightSourceInstance struct An instance of light source which contains the light source
 * parameters and metadata.
 */
struct LightSourceInstance {
    // A light source defined in the view space.
    LightSource light_source;
};

/**
 * @brief ToLightSources Extracts light sources from the scene entity list and transform them into
 * the view space.
 */
std::vector<LightSourceInstance>
ToLightSources(std::vector<SceneEntity const *> const &scene_entities,
               ProjectionInterface const &camera_projection);

} // namespace e8

#endif // ISLANDS_RENDERER_LIGHT_SOURCE_H
