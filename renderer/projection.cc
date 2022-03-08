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

#include <cmath>

#include "common/tensor.h"
#include "content/common.h"
#include "content/proto/camera.pb.h"
#include "renderer/projection.h"

namespace e8 {
namespace {

float const kNearClipDistance = 0.2f;

vec3 ToRadianRollPitchYaw(Camera const &camera) {
    return vec3{
        deg2rad(camera.rotation(0)),
        deg2rad(camera.rotation(1)),
        deg2rad(camera.rotation(2)),
    };
}

} // namespace

ProjectionInterface::ProjectionInterface() {}

ProjectionInterface::~ProjectionInterface() {}

PerspectiveProjection::PerspectiveProjection(float near_clip, float far_clip, float width,
                                             float height, vec3 const &location,
                                             vec3 const &roll_pitch_yaw)
    : frustum_(/*left=*/-width / 2.0f, /*right=*/width / 2.0f, /*top=*/height / 2.0f,
               /*bottom=*/-height / 2.0f, /*z_near=*/near_clip, /*z_far=*/far_clip),
      view_transform_(mat44_roll_pitch_yaw(location, roll_pitch_yaw)) {}

PerspectiveProjection::~PerspectiveProjection() {}

PerspectiveProjection::PerspectiveProjection(Camera const &camera)
    : PerspectiveProjection(kNearClipDistance, camera.max_distance(),
                            kNearClipDistance / camera.focal_length() * camera.sensor_width(),
                            kNearClipDistance / camera.focal_length() * camera.sensor_height(),
                            ToVec3(camera.position()), ToRadianRollPitchYaw(camera)) {}

mat44 PerspectiveProjection::ViewTransform() const { return view_transform_; }

mat44 PerspectiveProjection::ProjectiveTransform() const { return frustum_.projective_transform(); }

frustum const &PerspectiveProjection::Frustum() const { return frustum_; }

} // namespace e8
