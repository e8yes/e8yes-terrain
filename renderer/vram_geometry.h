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

#include <limits>
#include <memory>
#include <optional>

#include "content/proto/geometry.pb.h"
#include "renderer/context.h"
#include "renderer/projection.h"

namespace e8 {

/**
 * @brief The GeometryVramTransfer class It enables an efficient geometry data transfer from the
 * host machine to the GPU device as well as geometry data storage on the GPU device.
 */
class GeometryVramTransfer {
  public:
    /**
     * @brief GeometryVramTransfer Constructs a VRAM transferer.
     *
     * @param context A vulkan context pointing to the GPU device where data will transferred.
     * @param capacity Optionally specifies the maximum number of bytes the transferer will use on
     * the GPU device.
     */
    GeometryVramTransfer(VulkanContext *context,
                         unsigned capacity = std::numeric_limits<unsigned>::max());
    ~GeometryVramTransfer();

    /**
     * @brief The BufferUploadResult struct Contains information of an allocated buffer object.
     */
    struct BufferUploadResult {
        BufferUploadResult();
        ~BufferUploadResult();

        // The allocated buffer object.
        VkBuffer buffer;

        // Information about the buffer allocation.
        VmaAllocation allocation;

        // The size of the memory region of the buffer object.
        unsigned size;
    };

    /**
     * @brief The UploadResult struct Results of the Upload() call.
     */
    struct UploadResult {
        UploadResult();
        ~UploadResult();

        // A valid vertex buffer allocation if not null.
        std::optional<BufferUploadResult> vertex_buffer;

        // A valid index buffer allocation if not null.
        std::optional<BufferUploadResult> index_buffer;

        // The integer type each index element uses in the upload.
        VkIndexType index_element_type;
    };

    /**
     * @brief Upload Transfers the vertex and index data of the geometry to the GPU device if it has
     * not been cached or the geometry is mutable. A geometry is mutable if its rigidity type is
     * deformable or tearable. If the geometry data have been cached, it returns the previous
     * transfer result.
     *
     * @param geometry The geometry to transfer/update.
     * @return See the UploadResult structure above.
     */
    UploadResult Upload(Geometry const *geometry);

  private:
    class GeometryVramTransferImpl;
    std::unique_ptr<GeometryVramTransferImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_VRAM_GEOMETRY_H
