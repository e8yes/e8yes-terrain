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

#ifndef ISLANDS_RENDERER_DRAWABLE_INSTANCE_H
#define ISLANDS_RENDERER_DRAWABLE_INSTANCE_H

#include <vector>

#include "common/tensor.h"
#include "content/scene_entity.h"
#include "resource/accessor.h"
#include "resource/geometry.h"
#include "resource/light_map.h"
#include "resource/material.h"

namespace e8 {

/**
 * @brief The DrawableInstance struct Stores references to a geometry, material definition and
 * transformation. This instanced drawable is what get actually rendered.
 */
struct DrawableInstance {
    // The geometry selected from a series of LODs.
    Geometry const *geometry;

    // Optional. The material selected from a series of LODs.
    Material const *material;

    // Optional. The light map selected from a series of LODs.
    LightMap const *light_map;

    // Optional. The indirect light map selected from a series of LODs.
    LightMap const *indirect_light_map;

    // The homogeneous transformation to be applied to the geometry.
    mat44 const *transform;
};

/**
 * @brief The ResourceLoadingOption struct Options to supply to the ToDrawables() call.
 */
struct ResourceLoadingOption {
    /**
     * @brief ResourceLoadingOption All fields are default to false.
     */
    ResourceLoadingOption();
    ~ResourceLoadingOption();

    // Whether to attach geometry resource to the drawables.
    bool load_geometry;

    // Whether to attach material resource to the drawables.
    bool load_material;

    // Whether to attach light map resource to the drawables.
    bool load_light_map;

    // Whether to attach indirect light map resource to the drawables.
    bool load_indirect_light_map;
};

/**
 * @brief ToDrawables Selects a suitable resource LODs from each entity and creates a
 * drawable from the selection, if there is one. Scene entities that don't have a geometry reference
 * are excluded from the result.
 *
 * @param scene_entities The scene entities to create drawables from.
 * @param viewer_location Used for determining the resource LOD.
 * @param option Options to specify which resource to load.
 * @param resource_accessor Proxy to getting the actual resource data.
 * @return An array of drawables with applicable resources.
 */
std::vector<DrawableInstance> ToDrawables(std::vector<SceneEntity const *> const &scene_entities,
                                          vec3 const &viewer_location,
                                          ResourceLoadingOption const &option,
                                          ResourceAccessor *resource_accessor);

} // namespace e8

#endif // ISLANDS_RENDERER_DRAWABLE_INSTANCE_H
