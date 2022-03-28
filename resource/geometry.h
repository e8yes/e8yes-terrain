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

#include "common/device.h"
#include "resource/buffer_index.h"
#include "resource/buffer_vertex.h"
#include "resource/common.h"
#include "resource/proto/geometry.pb.h"
#include "resource/proto/table.pb.h"

namespace e8 {

// It uniquely identifies a geometry design.
using GeometryId = Uuid;

// A descriptive human readable name for a geometry.
using GeometryName = std::string;

/**
 * @brief The Geometry struct Models an object's geometry as triangle mesh. This object isn't thread
 * safe.
 */
struct Geometry {
    /**
     * @brief Geometry Constructs an empty geometry with a uniquely allocated ID.
     */
    Geometry();
    Geometry(Geometry const &) = delete;
    Geometry(Geometry &&other) = default;
    ~Geometry();

    Geometry &operator=(Geometry &&other) = default;

    /**
     * @brief FromDisk Recovers data from disk. If the data isn't available, this function will
     * fail. It uses the specified ID to look up resource metadata from the resource table, then it
     * recovers the geometries from data on disk to Vulkan buffer memory associated with the Vulkan
     * context.
     */
    void FromDisk(GeometryId const &id, ResourceTable const &table, VulkanContext *context);

    /**
     * @brief ToDisk It saves the geometry data to disk and populates the geometry resource's
     * metadata to the specified resource table.
     *
     * @param temporary Indicates if the geometry resource is only temporary. This allows the
     * resource table to conduct cleanup operations properly.
     */
    void ToDisk(bool temporary, ResourceTable *table) const;

    // ID of this geometry.
    GeometryId id;

    // The type of rigidity to be expected for this geometry.
    GeometryProto::RigidityType rigidity;

    // A valid host-only vertex buffer.
    StagingVertexBuffer vertices;

    // A valid host-only index buffer. Every 3 integers in the list define a triangle primitive. The
    // integer references a vertex from the vertex list.
    StagingIndexBuffer indices;
};

/**
 * @brief SaveGeometryProto Saves the specified geometry proto to disk and populates the geometry
 * resource's metadata to the specified resource table.
 *
 * @param temporary Indicates if the geometry resource is only temporary. This allows the
 * resource table to conduct cleanup operations properly.
 */
void SaveGeometryProto(GeometryProto const &proto, bool temporary, ResourceTable *table);

} // namespace e8

#endif // ISLANDS_CONTENT_GEOMETRY_H
