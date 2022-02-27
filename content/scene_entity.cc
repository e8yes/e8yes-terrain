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

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/tensor.h"
#include "content/common.h"
#include "content/drawable.h"
#include "content/proto/drawable.pb.h"
#include "content/proto/entity.pb.h"
#include "content/proto/physical_shape.pb.h"
#include "content/proto/primitive.pb.h"
#include "content/scene_entity.h"

namespace e8 {

SceneEntity::SceneEntity(SceneEntityName const &name)
    : id(GenerateUuid()), name(name), movable(true) {}

SceneEntity::SceneEntity(
    SceneEntityProto const &proto,
    std::unordered_map<DrawableId, std::shared_ptr<DrawableLod>> const &drawables)
    : id(proto.id()), name(proto.name()), movable(proto.movable()),
      transform(ToMat44(proto.transform())), bounding_box(ToAabb(proto.bounding_box())),
      drawable_lod_instance(drawables.at(proto.id())) {
    if (proto.has_srt_transform()) {
        srt_transform = proto.srt_transform();
    }
}

SceneEntity::~SceneEntity() {}

SceneEntityProto SceneEntity::ToProto() const {
    SceneEntityProto proto;
    proto.set_id(id);
    proto.set_name(name);
    proto.set_movable(movable);
    *proto.mutable_transform() = e8::ToProto(transform);
    if (srt_transform.has_value()) {
        *proto.mutable_srt_transform() = *srt_transform;
    }
    *proto.mutable_bounding_box() = e8::ToProto(bounding_box);
    proto.set_drawable_id(drawable_lod_instance->id());
    return proto;
}

void SceneEntitySetSrtTransform(SrtTransform const &srt_transform, SceneEntity *entity) {
    entity->srt_transform = srt_transform;
    entity->transform = ToHomogeneousTransform(srt_transform);
}

void SceneEntitySetTransform(mat44 const &transform, SceneEntity *entity) {
    entity->transform = transform;
    entity->srt_transform.reset();
}

} // namespace e8
