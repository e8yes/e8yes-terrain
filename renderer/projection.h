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

#ifndef ISLANDS_RENDERER_PROJECTION_H
#define ISLANDS_RENDERER_PROJECTION_H

#include "common/tensor.h"

namespace e8 {

/**
 * @brief The PerspectiveProjection class Represents the geometry and view transform of a
 * perspective projection.
 */
class PerspectiveProjection {
  public:
    /**
     * @brief PerspectiveProjection Constructs a perspective transform by specifying its geometry
     * parameters.
     *
     * @param near_clip The distance of the viewport at the direction from the pivot.
     * @param far_clip The farthest distance a geometry can be at and still gets rendered.
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     * @param location The 3D location of the pivot.
     * @param direction A unit vector defining the outward facing direction of the viewport.
     */
    PerspectiveProjection(float near_clip, float far_clip, float width, float height,
                          vec3 const &location, vec3 const &direction);
    ~PerspectiveProjection();

    /**
     * @brief Frustum The frustum geometry of the perspective projection.
     */
    frustum const &Frustum() const;

    /**
     * @brief Transform The view transform of the perspective projection.
     */
    mat44 const &Transform() const;

  private:
    frustum frustum_;
    mat44 view_transform_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PROJECTION_H
