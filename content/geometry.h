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

#ifndef ISLANDS_CONTENT_GEOMETRY_H
#define ISLANDS_CONTENT_GEOMETRY_H

#include <string>
#include <vector>

#include "common/tensor.h"
#include "content/proto/geometry.pb.h"

namespace e8 {

// It uniquely identifies a geometry design with its LOD series.
using GeometryId = std::string;

// A descriptive human readable name of a geometry LOD series.
using GeometryName = std::string;

/**
 * @brief The PrimitiveVertex struct It defines the geometric attributes of a primitive's
 * vertex.
 */
struct PrimitiveVertex {
    /**
     * @brief PrimitiveVertex Constructs a vertex with all attributes set to zero.
     */
    PrimitiveVertex();

    /**
     * @brief PrimitiveVertex Constructs a vertex from proto message.
     */
    PrimitiveVertex(PrimitiveVertexProto const &proto);

    ~PrimitiveVertex();

    /**
     * @brief ToProto Exports vertex attribute data as protobuf object.
     */
    PrimitiveVertexProto ToProto() const;

    // Defines the vertex's 3D position.
    vec3 position;

    // Defines the 3D normal vector for the vertex position.
    vec3 normal;

    // Defines the normalized 2D coordinates of a texture image to make it wrap around the surface
    // surrounding this vertex.
    vec2 tex_coord;
};

/**
 * @brief The Primitive struct Defines a triangle with vertices referencing from a
 * PrimitiveVertex list.
 */
struct Primitive {
    /**
     * @brief Primitive Constructs a degenerated primitive (3 vertices pointing to the same first
     * element in a PrimitiveVertex list).
     */
    Primitive();

    /**
     * @brief Primitive Constructs a primitive from proto message.
     */
    Primitive(PrimitiveIndicesProto const &proto);

    ~Primitive();

    /**
     * @brief ToProto Exports the geometry's data as protobuf object.
     */
    PrimitiveIndicesProto ToProto() const;

    // 3 vertices of a triangle face with attributes referenced from PrimitiveVertex.
    vec<3, unsigned> vertex_refs;
};

/**
 * @brief The Geometry struct Models an object's geometry as triangle mesh.
 */
struct Geometry {
    /**
     * @brief Geometry Constructs an empty geometry with a uniquely allocated ID.
     */
    Geometry();

    /**
     * @brief Geometry Constructs a geometry from proto message.
     */
    Geometry(GeometryProto const &proto);

    ~Geometry();

    /**
     * @brief ToProto Exports the geometry's data as protobuf object.
     */
    GeometryProto ToProto() const;

    // The type of rigidity to be expected for this geometry.
    GeometryProto::RigidityType rigidity;

    // A list of vertices defining the geometry of this geometry.
    std::vector<PrimitiveVertex> vertices;

    // Defines triangle primitives by referencing the vertices of each triangle from the vertex
    // list returned by the above function.
    std::vector<Primitive> primitives;
};

/**
 * @brief The GeometryLod struct Allows complex geometry to be represented with less detail at
 * distance.
 */
struct GeometryLod {
    /**
     * @brief GeometryLod Creates a geometry with LOD.
     *
     * @param name A descriptive human readable name for the geometry.
     * @param lod A series of geometry LOD in decreasing detail.
     * @param lod_min_distances The minimum distance from the viewer the ith LOD can be used.
     */
    GeometryLod(GeometryName const &name, std::vector<Geometry> const &lod,
                std::vector<float> lod_min_distances);

    /**
     * @brief GeometryLod Constructs a geometry LOD from proto message.
     */
    GeometryLod(GeometryLodProto const &proto);

    ~GeometryLod();

    /**
     * @brief ToProto Exports the geometry LOD data as protobuf object.
     */
    GeometryLodProto ToProto() const;

    // ID of this geometry design.
    GeometryId id;

    // A descriptive human readable name of this geometry.
    GeometryName name;

    // LOD of this geometry design in descending level of detail.
    std::vector<Geometry> lod;

    // A list the same length as the geometry_lod specifying the minimum distance the geometry LOD
    // is from the viewer before it can be applied to the rendering process. Therefore, the
    // distances are expected to be in ascending order. The renderer will pick the geometry with the
    // least possible detail when possible.
    std::vector<float> lod_min_distances;
};

} // namespace e8

#endif // ISLANDS_CONTENT_GEOMETRY_H
