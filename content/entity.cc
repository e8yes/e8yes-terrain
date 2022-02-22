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

#include <google/protobuf/map.h>
#include <google/protobuf/repeated_field.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/tensor.h"
#include "content/drawable.h"
#include "content/entity.h"
#include "content/proto/drawable.pb.h"
#include "content/proto/entity.pb.h"
#include "content/proto/physical_shape.pb.h"
#include "content/proto/primitive.pb.h"

namespace e8 {
namespace {

google::protobuf::RepeatedField<float> ToProtoArray(mat44 const &transform) {
    google::protobuf::RepeatedField<float> array;
    array.Resize(/*new_size=*/4 * 4, /*value=*/0.0f);

    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            array[i + j * 4] = transform(i, j);
        }
    }

    return array;
}

mat44 ToMat44(google::protobuf::RepeatedField<float> const &array) {
    mat44 transform;

    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            transform(i, j) = array[i + j * 4];
        }
    }

    return transform;
}

AABB ToProtoAabb(aabb const &bounding_box) {
    AABB proto;
    proto.mutable_min()->Resize(/*new_size=*/3, /*value=*/0.0f);
    proto.mutable_max()->Resize(/*new_size=*/3, /*value=*/0.0f);

    for (unsigned i = 0; i < 3; ++i) {
        proto.set_min(i, bounding_box.min()(i));
        proto.set_max(i, bounding_box.max()(i));
    }

    return proto;
}

aabb ToAabb(AABB const &proto) {
    vec3 min;
    vec3 max;

    for (unsigned i = 0; i < 3; ++i) {
        min(i) = proto.min(i);
        max(i) = proto.max(i);
    }

    return aabb(min, max);
}

} // namespace

SceneEntity::SceneEntity() {}

SceneEntity::SceneEntity(
    SceneEntityProto const &proto,
    std::unordered_map<DrawableId, std::shared_ptr<DrawableLod>> const &drawables)
    : id(proto.id()), name(proto.name()), movable(proto.movable()),
      transform(ToMat44(proto.transform())), bounding_box(ToAabb(proto.bounding_box())),
      drawable_lod_instance(drawables.at(proto.id())) {}

SceneEntity::~SceneEntity() {}

SceneEntityProto SceneEntity::ToProto() const {
    SceneEntityProto proto;
    proto.set_id(id);
    proto.set_name(name);
    proto.set_movable(movable);
    *proto.mutable_transform() = ToProtoArray(transform);
    *proto.mutable_bounding_box() = ToProtoAabb(bounding_box);
    proto.set_drawable_id(drawable_lod_instance->id());
    return proto;
}

SceneEntityCollection ToProto(std::vector<SceneEntity> const &entities) {
    SceneEntityCollection proto;

    for (auto const &entity : entities) {
        *proto.add_entities() = entity.ToProto();
        (*proto.mutable_drawables())[entity.drawable_lod_instance->id()] =
            *entity.drawable_lod_instance;
    }

    return proto;
}

std::vector<SceneEntity> ToSceneEntities(SceneEntityCollection const &proto) {
    std::unordered_map<DrawableId, std::shared_ptr<DrawableLod>> drawables;
    for (auto const &[id, drawable] : proto.drawables()) {
        drawables[id] = std::make_shared<DrawableLod>(drawable);
    }

    std::vector<SceneEntity> entities;
    for (auto entity_proto : proto.entities()) {
        entities.push_back(SceneEntity(entity_proto, drawables));
    }

    return entities;
}

} // namespace e8
