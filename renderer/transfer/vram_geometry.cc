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
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/cache.h"
#include "common/device.h"
#include "renderer/transfer/vram.h"
#include "renderer/transfer/vram_geometry.h"
#include "resource/buffer_index.h"
#include "resource/buffer_vertex.h"
#include "resource/geometry.h"

namespace e8 {
namespace {

void UploadVertexBuffer(Geometry const *geometry, uint64_t old_buffer_size,
                        uint64_t new_buffer_size, VulkanContext *context,
                        GeometryVramTransfer::GpuBuffer *vertex_buffer_object,
                        VkCommandBuffer cmds) {
    // Ensures the geometry's vertex buffer was allocated under the same context.
    assert(geometry->vertices.Valid());
    assert(geometry->vertices.context == context);

    if (new_buffer_size != old_buffer_size) {
        vertex_buffer_object->Allocate(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, new_buffer_size, context);
    }

    VkBufferCopy region{};
    region.size = geometry->vertices.BufferSize();

    vkCmdCopyBuffer(cmds, geometry->vertices.buffer, vertex_buffer_object->buffer,
                    /*regionCount=*/1, &region);
}

void UploadIndexBuffer(Geometry const *geometry, uint64_t old_buffer_size, uint64_t new_buffer_size,
                       VulkanContext *context, GeometryVramTransfer::GpuBuffer *index_buffer_object,
                       VkCommandBuffer cmds) {
    // Ensures the geometry's index buffer was allocated under the same context.
    assert(geometry->indices.Valid());
    assert(geometry->indices.context == context);

    if (new_buffer_size != old_buffer_size) {
        index_buffer_object->Allocate(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, new_buffer_size, context);
    }

    VkBufferCopy region{};
    region.size = geometry->indices.BufferSize();

    vkCmdCopyBuffer(cmds, geometry->indices.buffer, index_buffer_object->buffer, /*regionCount=*/1,
                    &region);
}

void UploadGeometry(Geometry const *geometry,
                    DeviceCache<Geometry const *, VramTransfer::GpuBuffer> *vertex_cache,
                    DeviceCache<Geometry const *, VramTransfer::GpuBuffer> *index_cache,
                    VulkanContext *context, VkCommandBuffer cmds) {
    vertex_cache->Upload(
        geometry,
        /*override_old_upload=*/geometry->rigidity == GeometryProto::DEFORMABLE ||
            geometry->rigidity == GeometryProto::TEARABLE,
        /*object_size_fn=*/[](Geometry const *geo) { return geo->vertices.BufferSize(); },
        /*upload_fn=*/
        [context, cmds](Geometry const *geometry, uint64_t old_buffer_size,
                        uint64_t new_buffer_size,
                        GeometryVramTransfer::GpuBuffer *vertex_buffer_object) {
            UploadVertexBuffer(geometry, old_buffer_size, new_buffer_size, context,
                               vertex_buffer_object, cmds);
        });

    index_cache->Upload(
        geometry, /*override_old_upload=*/geometry->rigidity == GeometryProto::TEARABLE,
        /*object_size_fn=*/[](Geometry const *geo) { return geo->indices.BufferSize(); },
        /*upload_fn=*/
        [context, cmds](Geometry const *geometry, uint64_t old_buffer_size,
                        uint64_t new_buffer_size,
                        GeometryVramTransfer::GpuBuffer *vertex_buffer_object) {
            UploadIndexBuffer(geometry, old_buffer_size, new_buffer_size, context,
                              vertex_buffer_object, cmds);
        });
}

} // namespace

GeometryVramTransfer::GeometryVramTransfer(VulkanContext *context)
    : VramTransfer(context), vertex_cache_(&context->geometry_vram_usage),
      index_cache_(&context->geometry_vram_usage) {}

GeometryVramTransfer::~GeometryVramTransfer() {}

GeometryVramTransfer::GpuGeometry::GpuGeometry()
    : vertex_buffer(nullptr), index_buffer(nullptr),
      index_element_type(VkIndexType::VK_INDEX_TYPE_MAX_ENUM) {}

GeometryVramTransfer::GpuGeometry::~GpuGeometry() {}

bool GeometryVramTransfer::GpuGeometry::Valid() const {
    return vertex_buffer != nullptr && vertex_buffer->Valid() && index_buffer != nullptr &&
           index_buffer->Valid();
}

void GeometryVramTransfer::Prepare() {
    vertex_cache_.Reset();
    index_cache_.Reset();
}

void GeometryVramTransfer::Upload(std::vector<Geometry const *> const &geometries) {
    VkCommandBuffer cmds = VramTransfer::BeginUpload();

    for (auto geometry : geometries) {
        UploadGeometry(geometry, &vertex_cache_, &index_cache_, context_, cmds);
    }

    VramTransfer::EndUpload(cmds);
}

GeometryVramTransfer::GpuGeometry GeometryVramTransfer::Find(Geometry const *geometry) {
    GpuGeometry result;

    result.vertex_buffer = vertex_cache_.Find(geometry);
    result.index_buffer = index_cache_.Find(geometry);
    assert(result.vertex_buffer != nullptr);
    assert(result.index_buffer != nullptr);

    result.index_element_type = geometry->indices.IndexElementType();

    return result;
}

} // namespace e8
