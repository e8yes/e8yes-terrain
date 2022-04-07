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

#ifndef ISLANDS_RENDERER_VRAM_GEOMETRY_H
#define ISLANDS_RENDERER_VRAM_GEOMETRY_H

#include <vector>
#include <vulkan/vulkan.h>

#include "common/cache.h"
#include "common/device.h"
#include "renderer/transfer/vram.h"
#include "resource/geometry.h"

namespace e8 {

/**
 * @brief The GeometryVramTransfer class It encapsulates an efficient and scalable geometry data
 * transfer from host machine to GPU device via caching. This class isn't thread-safe.
 */
class GeometryVramTransfer : public VramTransfer {
  public:
    /**
     * @brief GeometryVramTransfer Constructs a VRAM transferer.
     *
     * @param context A vulkan context pointing to the GPU device where data will transferred.
     * @param capacity Optionally specifies the maximum number of bytes the transferer will use on
     * the GPU device.
     */
    GeometryVramTransfer(VulkanContext *context);
    ~GeometryVramTransfer();

    /**
     * @brief The GpuGeometry struct Represents a geometry living on the video memory.
     */
    struct GpuGeometry {
        GpuGeometry();
        ~GpuGeometry();

        /**
         * @brief Valid Indicates if the upload is valid.
         */
        bool Valid() const;

        // A valid vertex buffer allocation if not null.
        GpuBuffer *vertex_buffer;

        // A valid index buffer allocation if not null.
        GpuBuffer *index_buffer;

        // The integer type each index element uses in the upload.
        VkIndexType index_element_type;
    };

    /**
     * @brief Prepare Calls this when all the uploaded GPU geometries are no longer needed, and
     * therefore, are allowed to be ejected.
     */
    void Prepare();

    /**
     * @brief Upload Transfers the vertex and index data of the specified geometry list to the GPU
     * device for those that have not been there. However, a mutable geometry will always be
     * transferred regardless of its cache status. A geometry is mutable if its rigidity type is
     * deformable or tearable. This function blocks until the uploads, if there are any, are
     * complete.
     *
     * @param geometries The geometries to be uploaded.
     */
    void Upload(std::vector<Geometry const *> const &geometries);

    /**
     * @brief Find Returns the uploaded geometry. If the specified geometry has not been uploaded,
     * this function will fail.
     */
    GpuGeometry Find(Geometry const *geometry);

  private:
    DeviceCache<Geometry const *, GpuBuffer> vertex_cache_;
    DeviceCache<Geometry const *, GpuBuffer> index_cache_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_VRAM_GEOMETRY_H
