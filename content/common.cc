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
#include <google/protobuf/repeated_field.h>

#include "common/tensor.h"
#include "content/common.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

std::mutex gUuidGenMutex;
bool gUuidInitialized = false;
UUID4_STATE_T gUuidGenState;

} // namespace

std::string GenerateUuid() {
    std::lock_guard guard(gUuidGenMutex);

    if (!gUuidInitialized) {
        uuid4_seed(&gUuidGenState);
        gUuidInitialized = true;
    }

    UUID4_T uuid;
    uuid4_gen(&gUuidGenState, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return std::string(uuid_string);
}

google::protobuf::RepeatedField<float> ToProto(vec2 const &v) {
    google::protobuf::RepeatedField<float> proto;
    proto.Resize(/*new_size=*/2, /*value=*/0);
    for (unsigned i = 0; i < 2; ++i) {
        proto[i] = v(i);
    }
    return proto;
}

vec2 ToVec2(google::protobuf::RepeatedField<float> const &proto) {
    assert(proto.size() == 2);

    vec2 v;
    for (unsigned i = 0; i < 2; ++i) {
        v(i) = proto[i];
    }
    return v;
}

google::protobuf::RepeatedField<float> ToProto(vec3 const &v) {
    google::protobuf::RepeatedField<float> proto;
    proto.Resize(/*new_size=*/3, /*value=*/0);
    for (unsigned i = 0; i < 3; ++i) {
        proto[i] = v(i);
    }
    return proto;
}

vec3 ToVec3(google::protobuf::RepeatedField<float> const &proto) {
    assert(proto.size() == 3);

    vec3 v;
    for (unsigned i = 0; i < 3; ++i) {
        v(i) = proto[i];
    }
    return v;
}

google::protobuf::RepeatedField<float> ToProto(mat44 const &m) {
    google::protobuf::RepeatedField<float> proto;
    proto.Resize(/*new_size=*/4 * 4, /*value=*/0);
    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            proto[i + j * 4] = m(i, j);
        }
    }
    return proto;
}

mat44 ToMat44(google::protobuf::RepeatedField<float> const &proto) {
    assert(proto.size() == 4 * 4);

    mat44 m;
    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            m(i, j) = proto[i + j * 4];
        }
    }
    return m;
}

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
