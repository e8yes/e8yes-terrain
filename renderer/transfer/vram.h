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

#ifndef ISLANDS_RENDERER_VRAM_H
#define ISLANDS_RENDERER_VRAM_H

#include <vulkan/vulkan.h>

#include "common/device.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

/**
 * @brief The VramTransfer class A VRAM transfer base class that provides common constructs to
 * facilitates data transfer from host to device.
 */
class VramTransfer {
  public:
    /**
     * @brief The GpuBuffer struct Contains information of an allocated buffer object living on the
     * VRAM.
     */
    struct GpuBuffer {
        GpuBuffer();
        GpuBuffer(GpuBuffer const &) = delete;
        GpuBuffer(GpuBuffer &&other);
        ~GpuBuffer();

        GpuBuffer &operator=(GpuBuffer &&other);

        void Allocate(VkBufferUsageFlags usage, unsigned size, VulkanContext *context);
        void Free();

        /**
         * @brief Valid Indicates if the buffer is valid.
         */
        bool Valid() const;

        // The allocated buffer object.
        VkBuffer buffer;

        // Buffer allocation handle.
        VmaAllocation allocation;

        // Contextual Vulkan handles.
        VulkanContext *context;
    };

    /**
     * @brief The GpuTexture struct Contains information of an allocated texture image object living
     * on the VRAM.
     */
    struct GpuTexture {
        GpuTexture();
        GpuTexture(GpuBuffer const &) = delete;
        GpuTexture(GpuTexture &&other);
        ~GpuTexture();

        GpuTexture &operator=(GpuTexture &&other);

        void Allocate(unsigned width, unsigned height, VkFormat image_format,
                      unsigned mip_level_count, VulkanContext *context);
        void Free();

        /**
         * @brief Valid Indicates if the image is valid.
         */
        bool Valid() const;

        // Holds the image data.
        VkImage image;

        // Image allocation handle.
        VmaAllocation allocation;

        // View over the image resource.
        VkImageView view;

        // Contextual Vulkan handles.
        VulkanContext *context;
    };

    /**
     * @brief VramTransfer Constructs a VRAM transfer base class upon the specified context.
     */
    VramTransfer(VulkanContext *context);
    ~VramTransfer();

  protected:
    VkCommandBuffer BeginUpload();
    void EndUpload(VkCommandBuffer cmds);

  protected:
    VulkanContext *context_;

  private:
    VkFence fence_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_VRAM_H
