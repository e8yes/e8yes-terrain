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
#include <vector>

#include "content/common.h"
#include "content/geometry.h"
#include "content/proto/geometry.pb.h"

namespace e8 {

PrimitiveVertex::PrimitiveVertex() {}

PrimitiveVertex::PrimitiveVertex(PrimitiveVertexProto const &proto)
    : position(ToVec3(proto.position())), normal(ToVec3(proto.normal())),
      tex_coord(ToVec2(proto.texcoord())) {}

PrimitiveVertex::~PrimitiveVertex() {}

PrimitiveVertexProto PrimitiveVertex::ToProto() const {
    PrimitiveVertexProto proto;
    *proto.mutable_position() = e8::ToProto(position);
    *proto.mutable_normal() = e8::ToProto(normal);
    *proto.mutable_texcoord() = e8::ToProto(tex_coord);
    return proto;
}

Primitive::Primitive() {}

Primitive::Primitive(PrimitiveIndicesProto const &proto) {
    for (unsigned i = 0; i < 3; ++i) {
        vertex_refs(i) = proto.indices(i);
    }
}

Primitive::~Primitive() {}

PrimitiveIndicesProto Primitive::ToProto() const {
    PrimitiveIndicesProto proto;
    for (unsigned i = 0; i < 3; ++i) {
        proto.set_indices(i, vertex_refs(i));
    }
    return proto;
}

Geometry::Geometry() {}

Geometry::Geometry(GeometryProto const &proto)
    : rigidity(proto.rigidity()), vertices(proto.vertices_size()),
      primitives(proto.primitives_size()) {
    for (int i = 0; i < proto.vertices_size(); ++i) {
        vertices[i] = PrimitiveVertex(proto.vertices(i));
    }
    for (int i = 0; i < proto.primitives_size(); ++i) {
        primitives[i] = Primitive(proto.primitives(i));
    }
}

Geometry::~Geometry() {}

GeometryProto Geometry::ToProto() const {
    GeometryProto proto;
    proto.set_rigidity(rigidity);
    for (unsigned i = 0; i < vertices.size(); ++i) {
        *proto.add_vertices() = vertices[i].ToProto();
    }
    for (unsigned i = 0; i < primitives.size(); ++i) {
        *proto.add_primitives() = primitives[i].ToProto();
    }
    return proto;
}

GeometryLod::GeometryLod(GeometryName const &name, std::vector<Geometry> const &lod,
                         std::vector<float> lod_min_distances)
    : id(GenerateUuid()), name(name), lod(lod), lod_min_distances(lod_min_distances) {}

GeometryLod::GeometryLod(GeometryLodProto const &proto)
    : id(proto.id()), name(proto.name()), lod(proto.geometry_lod_size()),
      lod_min_distances(proto.min_distances_size()) {
    for (int i = 0; i < proto.geometry_lod_size(); ++i) {
        lod[i] = Geometry(proto.geometry_lod(i));
    }
    for (int i = 0; i < proto.min_distances_size(); ++i) {
        lod_min_distances[i] = proto.min_distances(i);
    }
    assert(lod.size() == lod_min_distances.size());
}

GeometryLod::~GeometryLod() {}

GeometryLodProto GeometryLod::ToProto() const {
    GeometryLodProto proto;
    for (unsigned i = 0; i < lod.size(); ++i) {
        *proto.add_geometry_lod() = lod[i].ToProto();
    }
    for (unsigned i = 0; i < lod_min_distances.size(); ++i) {
        proto.set_min_distances(i, lod_min_distances[i]);
    }
    return proto;
}

} // namespace e8
