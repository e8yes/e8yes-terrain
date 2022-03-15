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

#include "content/camera.h"
#include "common/tensor.h"
#include "content/common.h"
#include "content/proto/camera.pb.h"

namespace e8 {
namespace {} // namespace

Camera DefaultCamera() {
    Camera camera;

    camera.set_id(GenerateUuid());
    *camera.mutable_position() = e8::ToProto(vec3{0, 0, 2});
    *camera.mutable_basis()->mutable_right() = e8::ToProto(vec3{1, 0, 0});
    *camera.mutable_basis()->mutable_down() = e8::ToProto(vec3{0, 1, 0});
    *camera.mutable_basis()->mutable_back() = e8::ToProto(vec3{0, 0, 1});
    camera.set_focal_length(35);
    camera.set_sensor_width(36);
    camera.set_sensor_height(24);
    camera.set_image_width(1024);
    camera.set_max_distance(1000);

    return camera;
}

void CameraRotateBasis(float roll, float pitch, float yaw, Camera *camera) {
    vec3 right = ToVec3(camera->basis().right());
    vec3 down = ToVec3(camera->basis().down());
    vec3 back = ToVec3(camera->basis().back());

    mat33 roll_transform = mat33_rotate(deg2rad(roll), back);
    right = roll_transform * right;
    down = roll_transform * down;

    mat33 pitch_transform = mat33_rotate(deg2rad(pitch), right);
    down = pitch_transform * down;
    back = pitch_transform * back;

    mat33 yaw_transform = mat33_rotate(deg2rad(yaw), down);
    right = yaw_transform * right;
    back = yaw_transform * back;

    down = back.outer(right);
    back = right.outer(down);

    *camera->mutable_basis()->mutable_right() = ToProto(right.normalize());
    *camera->mutable_basis()->mutable_down() = ToProto(down.normalize());
    *camera->mutable_basis()->mutable_back() = ToProto(back.normalize());
}

void CameraEnsureLeveling(Camera *camera) {
    vec3 right = ToVec3(camera->basis().right());
    vec3 down = ToVec3(camera->basis().down());
    vec3 back = ToVec3(camera->basis().back());

    right(2) = 0.0f;
    down = back.outer(right);
    back = right.outer(down);

    *camera->mutable_basis()->mutable_right() = ToProto(right.normalize());
    *camera->mutable_basis()->mutable_down() = ToProto(down.normalize());
    *camera->mutable_basis()->mutable_back() = ToProto(back.normalize());
}

} // namespace e8
