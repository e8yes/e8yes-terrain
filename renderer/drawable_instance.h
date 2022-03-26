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

namespace e8 {

/**
 * @brief The DrawableInstance struct Stores references to a geometry, material definition and
 * transformation. This instanced drawable is what get actually rendered.
 */
struct DrawableInstance {
    // The geometry selected from a series of LODs.
    Geometry const *geometry;

    // The homogeneous transformation to be applied to the geometry.
    mat44 const *transform;
};

/**
 * @brief ToDrawable Selects a suitable geometry and material LODs from each entity and creates a
 * drawable from the selection, if there is one.
 */
std::vector<DrawableInstance> ToDrawables(std::vector<SceneEntity const *> const &scene_entities,
                                          vec3 const &viewer_location,
                                          ResourceAccessor *resource_accessor);

} // namespace e8

#endif // ISLANDS_RENDERER_DRAWABLE_INSTANCE_H
