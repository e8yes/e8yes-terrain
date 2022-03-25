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
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "common/tensor.h"
#include "resource/common.h"
#include "resource/proto/geometry.pb.h"
#include "resource/proto/table.pb.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

// It uniquely identifies a geometry design with its LOD series.
using GeometryId = Uuid;

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
 * @brief The Geometry struct Models an object's geometry as triangle mesh. This object isn't thread
 * safe.
 */
struct Geometry {
    /**
     * @brief The VulkanBuffer struct Wraps on a Vulkan buffer with a simply memory management
     * layer. The Vulkan buffer is used for storing the geometry's vertex and index data.
     */
    struct VulkanBuffer {
        VulkanBuffer();
        VulkanBuffer(VulkanBuffer const &) = delete;
        VulkanBuffer(VulkanBuffer &&);
        ~VulkanBuffer();

        VulkanBuffer &operator=(VulkanBuffer &&other);

        /**
         * @brief Allocate Used internally by the Geometry object for Vulkan buffer allocation.
         */
        void Allocate(unsigned size, VulkanContext *context);

        /**
         * @brief Free Used internally by the Geometry object for Vulkan buffer de-allocation.
         */
        void Free();

        /**
         * @brief BeginAccess Obtains a pointer to the buffer region accessible by caller's process.
         */
        void *BeginAccess() const;

        /**
         * @brief EndAccess This must be called when the access to the buffer region is finished.
         */
        void EndAccess() const;

        /**
         * @brief Valid Indicates if the buffer is valid.
         */
        bool Valid() const;

        // The allocated buffer object.
        VkBuffer buffer;

        // Information about the buffer allocation.
        VmaAllocation allocation;

        // Contextual Vulkan handles.
        VulkanContext *context;
    };

    /**
     * @brief Geometry Constructs an empty geometry with a uniquely allocated ID.
     */
    Geometry();
    Geometry(Geometry const &) = delete;
    Geometry(Geometry &&other) = default;
    ~Geometry();

    Geometry &operator=(Geometry &&other) = default;

    /**
     * @brief FromProto Constructs a geometry from proto message.
     */
    void FromProto(GeometryProto const &proto, VulkanContext *context);

    /**
     * @brief ToProto Exports the geometry's data as protobuf object.
     */
    GeometryProto ToProto() const;

    /**
     * @brief IndexElementType The integer type each index element uses in the upload.
     */
    VkIndexType IndexElementType() const;

    /**
     * @brief IndexElementSize
     */
    unsigned IndexElementSize() const;

    /**
     * @brief VertexBufferSize
     */
    uint64_t VertexBufferSize() const;

    /**
     * @brief IndexBufferSize
     */
    uint64_t IndexBufferSize() const;

    // The type of rigidity to be expected for this geometry.
    GeometryProto::RigidityType rigidity;

    // A host-only vertex buffer. It's not guaranteed to be unique. The buffer physically contains a
    // list of vertices defining the geometry of this geometry.
    VulkanBuffer vertices;

    // The number of vertex there are in this geometry.
    unsigned vertex_count;

    // A valid host-only index buffer. It's not guaranteed to be unique. Every 3 integers in the
    // list define a triangle primitive. The integer references a vertex from the vertex list. The
    // size of the index element is indicated by the field index_element_type below.
    VulkanBuffer indices;

    // The number of index there are in this geometry.
    unsigned index_count;
};

/**
 * @brief The GeometryLod struct Allows complex geometry to be represented with less detail at
 * distance.
 */
struct GeometryLod {
    GeometryLod();
    GeometryLod(GeometryLod const &) = delete;
    GeometryLod(GeometryLod &&other) = default;
    ~GeometryLod();

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

/**
 * @brief SaveGeometryProto Saves the specified geometry LOD proto to disk and populates the
 * geometry resource's metadata to the specified resource table.
 *
 * @param temporary Indicates if the geometry resource is only temporary. This allows the
 * resource table to conduct cleanup operations properly.
 */
void SaveGeometryProto(GeometryLodProto const &proto, bool temporary, ResourceTable *table);

} // namespace e8

#endif // ISLANDS_CONTENT_GEOMETRY_H
