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
#include <string>

#include "common/tensor.h"
#include "content/common.h"
#include "content/proto/bbox.pb.h"
#include "content/proto/transform.pb.h"
#include "resource/common.h"

namespace e8 {

AABB ToProto(aabb const &a) {
    AABB proto;
    *proto.mutable_min() = ToProto(a.min());
    *proto.mutable_max() = ToProto(a.max());
    return proto;
}

aabb ToAabb(AABB const &proto) {
    vec3 min = ToVec3(proto.min());
    vec3 max = ToVec3(proto.max());
    return aabb(min, max);
}

mat44 ToHomogeneousTransform(SrtTransform const &srt_transform) {
    mat44 scaling;
    if (!srt_transform.scaling().empty()) {
        scaling = mat44_scale(ToVec3(srt_transform.scaling()));
    } else {
        scaling = mat44_identity();
    }

    mat44 rot_z;
    mat44 rot_y;
    mat44 rot_x;
    if (!srt_transform.rotation().empty()) {
        rot_z = mat44_rotate(deg2rad(srt_transform.rotation(2)), vec3{0, 0, 1});
        rot_y = mat44_rotate(deg2rad(srt_transform.rotation(1)), vec3{0, 1, 0});
        rot_x = mat44_rotate(deg2rad(srt_transform.rotation(0)), vec3{1, 0, 0});
    } else {
        rot_z = mat44_identity();
        rot_y = mat44_identity();
        rot_x = mat44_identity();
    }

    mat44 translation;
    if (!srt_transform.translation().empty()) {
        translation = mat44_translate(ToVec3(srt_transform.translation()));
    } else {
        translation = mat44_identity();
    }

    return translation * rot_z * rot_y * rot_x * scaling;
}

} // namespace e8
