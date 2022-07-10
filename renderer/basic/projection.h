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
#include "content/proto/camera.pb.h"

namespace e8 {

/**
 * @brief The ProjectionInterface class Represents a general projection operation, expressed in
 * terms of homogeneous transformations.
 */
class ProjectionInterface {
   public:
    ProjectionInterface();
    ~ProjectionInterface();

    /**
     * @brief ViewTransform Transforms world space coordinates to view space coordinates, where they
     * project towards the origin in the direction opposite of z axis.
     */
    virtual mat44 ViewTransform() const = 0;

    /**
     * @brief ProjectiveTransform Transforms view space coordinates to clip space (homogeneous)
     * coordinates.
     */
    virtual mat44 ProjectiveTransform() const = 0;
};

/**
 * @brief The PerspectiveProjection class Defines the geometry as well as view and projective
 * transform of a perspective projection.
 */
class PerspectiveProjection : public ProjectionInterface {
   public:
    /**
     * @brief PerspectiveProjection Constructs a perspective projection by specifying geometry
     * parameters of the projection frustum.
     *
     * @param near_clip The distance of the viewport at the direction from the pivot.
     * @param far_clip The farthest distance a geometry can be at and still gets rendered.
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     * @param location The 3D location of the pivot.
     * @param right
     * @param down
     * @param back
     */
    PerspectiveProjection(float near_clip, float far_clip, float width, float height,
                          vec3 const &location, vec3 const &right, vec3 const &down,
                          vec3 const &back);

    /**
     * @brief PerspectiveProjection Constructs a perspective projection from camera parameters.
     */
    PerspectiveProjection(Camera const &camera);

    ~PerspectiveProjection();

    mat44 ViewTransform() const override;

    mat44 ProjectiveTransform() const override;

    /**
     * @brief Location The 3d location of the tip of the projection frustum.
     */
    vec3 Location() const;

    /**
     * @brief Frustum The frustum geometry of the perspective projection.
     */
    frustum const &Frustum() const;

   private:
    frustum frustum_;
    mat44 view_transform_;
};

}  // namespace e8

#endif  // ISLANDS_RENDERER_PROJECTION_H
