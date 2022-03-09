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

#include "common/tensor.h"
#include "content/common.h"
#include "content/geometry.h"
#include "content/procedural_basic.h"
#include "content/procedural_object.h"
#include "content/proto/geometry.pb.h"
#include "content/proto/procedural_object.pb.h"
#include "content/proto/procedural_shape.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"

namespace e8 {
namespace {

constexpr char const *kEntityName = "entity";
constexpr char const *kGeometryName = "geometry";
float const kLodMinDistance = 0.0f;

aabb BoundingBoxOf(Geometry const &geometry) {
    aabb bounding_box;
    for (auto const &vertex : geometry.vertices) {
        bounding_box = bounding_box + vertex.position;
    }
    return bounding_box;
}

Geometry PlaneGeometry(float width, float height, float cell_area, bool movable) {
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

    Geometry geometry;

    // Generates vertices.
    geometry.vertices.resize(num_width_steps * num_height_steps);

    for (unsigned j = 0; j < num_height_steps; ++j) {
        for (unsigned i = 0; i < num_width_steps; ++i) {
            PrimitiveVertex vertex;
            vertex.position = vec3{quad_width * i - width / 2, height / 2 - quad_height * j, 0};
            vertex.normal = vec3{0, 0, 1};
            vertex.tex_coord = vec2{static_cast<float>(i) / (num_width_steps - 1),
                                    static_cast<float>(j) / (num_height_steps - 1)};

            geometry.vertices[i + j * num_width_steps] = vertex;
        }
    }

    // Generates triangle faces.
    geometry.primitives.resize((num_width_steps - 1) * (num_height_steps - 1) * 2);

    for (unsigned j = 0; j < num_height_steps - 1; ++j) {
        for (unsigned i = 0; i < num_width_steps - 1; ++i) {
            Primitive top_left;
            top_left.vertex_refs(0) = (i + 0) + (j + 0) * num_width_steps;
            top_left.vertex_refs(1) = (i + 1) + (j + 0) * num_width_steps;
            top_left.vertex_refs(2) = (i + 0) + (j + 1) * num_width_steps;

            Primitive bottom_right;
            bottom_right.vertex_refs(0) = (i + 1) + (j + 1) * num_width_steps;
            bottom_right.vertex_refs(1) = (i + 0) + (j + 1) * num_width_steps;
            bottom_right.vertex_refs(2) = (i + 1) + (j + 0) * num_width_steps;

            geometry.primitives[i + 0 + j * (num_width_steps - 1) * 2] = top_left;
            geometry.primitives[i + 1 + j * (num_width_steps - 1) * 2] = bottom_right;
        }
    }

    if (movable) {
        geometry.rigidity = GeometryProto::RIGID;
    } else {
        geometry.rigidity = GeometryProto::STATIC;
    }

    return geometry;
}

SceneEntity PlaneEntity(float width, float height, float cell_area,
                        SrtTransform const &srt_transform, bool movable) {
    Geometry plane_geometry = PlaneGeometry(width, height, cell_area, movable);

    std::vector<Geometry> lod{plane_geometry};
    std::vector<float> lod_min_distances{kLodMinDistance};

    std::shared_ptr<GeometryLod> geometry_lod_instance =
        std::make_shared<GeometryLod>(kGeometryName, lod, lod_min_distances);

    SceneEntity entity(kEntityName);
    entity.movable = movable;
    entity.bounding_box = BoundingBoxOf(geometry_lod_instance->lod[0]);
    SceneEntitySetSrtTransform(srt_transform, &entity);
    entity.geometry_lod_instance = geometry_lod_instance;

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

    ProceduralObjectProto proto = this->_ToBaseProto(ProceduralObjectProto::PLANE);
    *proto.add_shapes() = shape;
    return proto;
}

} // namespace e8
