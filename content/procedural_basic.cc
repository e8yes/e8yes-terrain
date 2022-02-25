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
#include <cmath>
#include <google/protobuf/repeated_field.h>
#include <memory>

#include "common/tensor.h"
#include "content/common.h"
#include "content/drawable.h"
#include "content/procedural_basic.h"
#include "content/procedural_object.h"
#include "content/proto/drawable.pb.h"
#include "content/proto/procedural_object.pb.h"
#include "content/proto/procedural_shape.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"

namespace e8 {
namespace {

constexpr char const *kEntityName = "entity";
constexpr char const *kDrawableName = "drawable";
float const kLodMinDistance = 0.0f;

Drawable PlaneDrawable(float width, float height, float cell_area, bool movable) {
    assert(width > 0);
    assert(height > 0);
    assert(cell_area <= width * height);

    float aspect_ratio = width / height;
    float quad_height = std::sqrt(cell_area / aspect_ratio);
    float quad_width = aspect_ratio * quad_height;

    unsigned num_width_steps = std::round(width / quad_width) + 1;
    unsigned num_height_steps = std::round(height / quad_height) + 1;

    assert(num_width_steps >= 2);
    assert(num_height_steps >= 2);

    Drawable drawable;

    // Generates vertices.
    for (unsigned j = 0; j < num_height_steps; ++j) {
        for (unsigned i = 0; i < num_width_steps; ++i) {
            vec3 position{quad_width * i - width / 2, quad_height * j - height / 2, 0};
            vec3 normal{0, 0, 1};
            vec2 tex_coord{static_cast<float>(i) / (num_width_steps - 1),
                           static_cast<float>(j) / (num_height_steps - 1)};

            PrimitiveVertex *vertex = drawable.add_vertices();
            *vertex->mutable_position() = ToProto(position);
            *vertex->mutable_normal() = ToProto(normal);
            *vertex->mutable_texcoord() = ToProto(tex_coord);
        }
    }

    // Generates triangle faces.
    for (unsigned j = 0; j < num_height_steps - 1; ++j) {
        for (unsigned i = 0; i < num_width_steps - 1; ++i) {
            PrimitiveIndices *top_left = drawable.add_primitives();
            top_left->add_indices((i + 0) + (j + 0) * num_width_steps);
            top_left->add_indices((i + 1) + (j + 0) * num_width_steps);
            top_left->add_indices((i + 0) + (j + 1) * num_width_steps);

            PrimitiveIndices *bottom_right = drawable.add_primitives();
            bottom_right->add_indices((i + 1) + (j + 1) * num_width_steps);
            bottom_right->add_indices((i + 0) + (j + 1) * num_width_steps);
            bottom_right->add_indices((i + 1) + (j + 0) * num_width_steps);
        }
    }

    if (movable) {
        drawable.set_rigidity(Drawable::RIGID);
    } else {
        drawable.set_rigidity(Drawable::STATIC);
    }

    return drawable;
}

SceneEntity PlaneEntity(float width, float height, float cell_area,
                        SrtTransform const &srt_transform, bool movable) {
    google::protobuf::RepeatedPtrField<Drawable> drawable_lod;
    google::protobuf::RepeatedField<float> min_distances;

    Drawable plane_drawable = PlaneDrawable(width, height, cell_area, movable);
    *drawable_lod.Add() = plane_drawable;
    min_distances.Add(kLodMinDistance);

    std::shared_ptr<DrawableLod> drawable_lod_instance =
        CreateDrawable(kDrawableName, drawable_lod, min_distances);

    SceneEntity entity(kEntityName);
    entity.movable = movable;
    SceneEntitySetSrtTransform(srt_transform, &entity);
    entity.drawable_lod_instance = drawable_lod_instance;

    return entity;
}

} // namespace

ProceduralPlane::ProceduralPlane(ProceduralObjectName const &name, float width, float height,
                                 float cell_area, SrtTransform const &srt_transform)
    : ProceduralObjectInterface(name), width(width), height(height), cell_area(cell_area),
      srt_transform(srt_transform) {}

ProceduralPlane::ProceduralPlane(ProceduralObjectProto const &proto)
    : ProceduralObjectInterface(proto) {}

ProceduralPlane::~ProceduralPlane() {}

SceneObject ProceduralPlane::ToSceneObject() const {
    SceneObject scene_object(name, id);

    SceneEntity entity = PlaneEntity(width, height, cell_area, srt_transform, movable);
    scene_object.AddSceneEntityChild(entity);

    return scene_object;
}

ProceduralObjectProto ProceduralPlane::ToProto() const {
    ProceduralPlaneProto plane;
    plane.set_cell_area(cell_area);
    plane.set_width(width);
    plane.set_height(height);

    ProceduralShapeProto shape;
    *shape.mutable_srt_transform() = srt_transform;
    *shape.mutable_plane() = plane;

    ProceduralObjectProto proto = this->_ToBaseProto();
    *proto.add_shapes() = shape;
    return proto;
}

} // namespace e8
