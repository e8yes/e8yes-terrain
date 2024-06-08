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
#include <cstring>
#include <google/protobuf/repeated_field.h>
#include <mutex>
#include <string>
#include <unordered_map>

#include "common/tensor.h"
#include "resource/common.h"
#include "resource/proto/primitive.pb.h"
#include "third_party/stb_image/stb_image_write.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

std::mutex gUuidGenMutex;
bool gUuidInitialized = false;
UUID4_STATE_T gUuidGenState;

std::mutex gUuidStringAssignmentMutex;
std::unordered_map<std::string, Uuid> gUuidStringAssignments;

void CopyEncodedImageFn(void *context, void *data, int size) {
    auto texture_proto = static_cast<TextureProto *>(context);
    texture_proto->mutable_data()->resize(size);
    memcpy(texture_proto->mutable_data()->data(), data, size);
}

} // namespace

Uuid GenerateUuid() {
    std::lock_guard guard(gUuidGenMutex);

    if (!gUuidInitialized) {
        uuid4_seed(&gUuidGenState);
        gUuidInitialized = true;
    }

    UUID4_T uuid;
    uuid4_gen(&gUuidGenState, &uuid);

    return uuid.qwords[0];
}

Uuid GenerateUuidFor(std::string const &s) {
    std::lock_guard guard(gUuidStringAssignmentMutex);

    auto it = gUuidStringAssignments.find(s);
    if (it != gUuidStringAssignments.end()) {
        return it->second;
    }

    Uuid allocated = GenerateUuid();
    gUuidStringAssignments.insert(std::make_pair(s, allocated));
    return allocated;
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

google::protobuf::RepeatedField<float> ToProto(vec4 const &v) {
    google::protobuf::RepeatedField<float> proto;
    proto.Resize(/*new_size=*/4, /*value=*/0);
    for (unsigned i = 0; i < 4; ++i) {
        proto[i] = v(i);
    }
    return proto;
}

vec4 ToVec4(google::protobuf::RepeatedField<float> const &proto) {
    assert(proto.size() == 4);

    vec4 v;
    for (unsigned i = 0; i < 4; ++i) {
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

void EncodeTextureData(void const *bitmap, TextureProto *texture_proto) {
    switch (texture_proto->encoding()) {
    case TextureProto::PNG: {
        stbi_write_png_to_func(CopyEncodedImageFn, /*context=*/texture_proto,
                               texture_proto->width(), texture_proto->height(),
                               texture_proto->channel_count(), bitmap,
                               texture_proto->width() * texture_proto->channel_count() *
                                   texture_proto->channel_size());
        break;
    }
    default: {
        assert(false);
    }
    }
}

} // namespace e8
