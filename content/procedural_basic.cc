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
#include <memory>

#include "common/device.h"
#include "common/tensor.h"
#include "content/common.h"
#include "content/procedural_basic.h"
#include "content/procedural_object.h"
#include "content/proto/lod.pb.h"
#include "content/proto/procedural_object.pb.h"
#include "content/proto/procedural_shape.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "resource/accessor.h"
#include "resource/common.h"
#include "resource/geometry.h"
#include "resource/proto/geometry.pb.h"

namespace e8 {
namespace {

constexpr char const *kEntityName = "entity";
float const kLodMinDistance = 0.0f;

GeometryProto PlaneGeometry(float width, float height, float cell_area, bool movable) {
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

    // Builds a geometry proto with the highest level of detail.
    GeometryProto geometry_proto;
    geometry_proto.set_id(GenerateUuid());
    geometry_proto.set_rigidity(GeometryProto::RIGID);

    // Generates vertices.
    for (unsigned j = 0; j < num_height_steps; ++j) {
        for (unsigned i = 0; i < num_width_steps; ++i) {
            PrimitiveVertexProto *vertex = geometry_proto.add_vertices();

            *vertex->mutable_position() =
                ToProto(vec3{quad_width * i - width / 2, height / 2 - quad_height * j, 0});
            *vertex->mutable_normal() = ToProto(vec3{0, 0, 1});
            *vertex->mutable_texcoord() =
                ToProto(vec2{static_cast<float>(i) / (num_width_steps - 1),
                             static_cast<float>(j) / (num_height_steps - 1)});
        }
    }

    // Generates triangle faces in counter-clockwise winding order.
    for (unsigned j = 0; j < num_height_steps - 1; ++j) {
        for (unsigned i = 0; i < num_width_steps - 1; ++i) {
            PrimitiveIndicesProto *top_left = geometry_proto.add_primitives();
            top_left->add_indices((i + 0) + (j + 0) * num_width_steps);
            top_left->add_indices((i + 0) + (j + 1) * num_width_steps);
            top_left->add_indices((i + 1) + (j + 0) * num_width_steps);

            PrimitiveIndicesProto *bottom_right = geometry_proto.add_primitives();
            bottom_right->add_indices((i + 1) + (j + 1) * num_width_steps);
            bottom_right->add_indices((i + 1) + (j + 0) * num_width_steps);
            bottom_right->add_indices((i + 0) + (j + 1) * num_width_steps);
        }
    }

    if (movable) {
        geometry_proto.set_rigidity(GeometryProto::RIGID);
    } else {
        geometry_proto.set_rigidity(GeometryProto::STATIC);
    }

    return geometry_proto;
}

SceneEntity PlaneEntity(float width, float height, float cell_area,
                        SrtTransform const &srt_transform, bool movable,
                        ResourceAccessor *resource_accessor) {
    // Materializes the plane resource into a form workable by the rest of the system.
    GeometryProto plane = PlaneGeometry(width, height, cell_area, movable);
    resource_accessor->AddGeometry(plane, /*temporary=*/true);

    // Constructs a scene entity wrapping around the plane resources.
    SceneEntity entity(kEntityName);
    entity.movable = movable;
    entity.bounding_box = BoundingBoxOf(plane);
    SceneEntityResources::Lod *lod = entity.resources.add_lods();
    lod->set_apply_after_distance(kLodMinDistance);
    lod->set_geometry_id(plane.id());

    SceneEntitySetSrtTransform(srt_transform, &entity);

    return entity;
}

} // namespace

ProceduralPlane::ProceduralPlane(ProceduralObjectName const &name, float width, float height,
                                 float cell_area)
    : ProceduralObjectInterface(name), width(width), height(height), cell_area(cell_area) {
    *srt_transform.mutable_scaling() = e8::ToProto(vec3{1, 1, 1});
    *srt_transform.mutable_rotation() = e8::ToProto(vec3{0, 0, 0});
    *srt_transform.mutable_translation() = e8::ToProto(vec3{0, 0, 0});
}

ProceduralPlane::ProceduralPlane(ProceduralObjectName const &name, float width, float height,
                                 float cell_area, SrtTransform const &srt_transform)
    : ProceduralPlane(name, width, height, cell_area) {
    this->srt_transform = srt_transform;
}

ProceduralPlane::ProceduralPlane(ProceduralObjectProto const &proto)
    : ProceduralObjectInterface(proto) {
    assert(proto.shapes().size() == 1);
    assert(proto.shapes(0).has_plane());

    srt_transform = proto.shapes(0).srt_transform();
    width = proto.shapes(0).plane().width();
    height = proto.shapes(0).plane().height();
    cell_area = proto.shapes(0).plane().cell_area();
}

ProceduralPlane::~ProceduralPlane() {}

SceneObject ProceduralPlane::ToSceneObject(ResourceAccessor *resource_accessor) const {
    SceneObject scene_object(name, id);

    SceneEntity entity =
        PlaneEntity(width, height, cell_area, srt_transform, movable, resource_accessor);
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

    ProceduralObjectProto proto = this->_ToBaseProto(ProceduralObjectProto::PLANE);
    *proto.add_shapes() = shape;
    return proto;
}

} // namespace e8
