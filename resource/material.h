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

#ifndef ISLANDS_RESOURCE_MATERIAL_H
#define ISLANDS_RESOURCE_MATERIAL_H

#include <string>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "resource/common.h"
#include "resource/proto/material.pb.h"
#include "resource/proto/table.pb.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

// It uniquely identifies a material design.
using MaterialId = Uuid;

// A descriptive human readable name for a material.
using MaterialName = std::string;

/**
 * @brief The Material struct It defines the appearance of the surface of entities that use this
 * material.
 */
struct Material {
    /**
     * @brief The VulkanImage struct Wraps on a Vulkan image buffer with a simply memory management
     * layer. The Vulkan image buffer is used for storing the material's textures.
     */
    struct VulkanImage {
        VulkanImage();
        VulkanImage(VulkanImage const &) = delete;
        VulkanImage(VulkanImage &&other);
        ~VulkanImage();

        VulkanImage &operator=(VulkanImage &&other);

        /**
         * @brief Allocate Used internally by the Material object for Vulkan image buffer
         * allocation.
         */
        void Allocate(unsigned width, unsigned height, unsigned channel_size,
                      unsigned channel_count, VulkanContext *context);

        /**
         * @brief Free Used internally by the Material object for Vulkan image buffer de-allocation.
         */
        void Free();

        /**
         * @brief BeginAccess Obtains a pointer to the image buffer region accessible by caller's
         * process.
         */
        void *BeginAccess() const;

        /**
         * @brief EndAccess This must be called when the access to the image buffer region is
         * finished.
         */
        void EndAccess() const;

        /**
         * @brief Valid Indicates if the image buffer is valid.
         */
        bool Valid() const;

        /**
         * @brief ImageSize Returns the size of this image buffer, in byte.
         */
        unsigned ImageSize() const;

        // The allocated image buffer object.
        VkBuffer buffer;

        // Information about the image buffer allocation.
        VmaAllocation allocation;

        // The width of the texture image.
        unsigned width;

        // The height of the texture image.
        unsigned height;

        // The number of channel each image pixel carries.
        unsigned channel_size;

        // The number of byte each channel uses.
        unsigned channel_count;

        // Contextual Vulkan handles.
        VulkanContext *context;
    };

    Material();
    Material(Material const &) = delete;
    Material(Material &&) = default;
    ~Material();

    /**
     * @brief FromDisk Recovers data from disk. If the data isn't available, this function will
     * fail. It uses the specified ID to look up resource metadata from the resource table, then it
     * recovers the material from data on disk to Vulkan buffer memory associated with the Vulkan
     * context.
     */
    void FromDisk(MaterialId const &id, ResourceTable const &table, VulkanContext *context);

    // ID of this material design.
    MaterialId id;

    // A descriptive human readable name of this material.
    MaterialName name;

    // The albedo map.
    VulkanImage albedo;

    // The normal map.
    VulkanImage normal;

    // The roughness map.
    VulkanImage roughness;
};

/**
 * @brief SaveMaterialProto Saves the specified material proto to disk and populates the material
 * resource's metadata to the specified resource table.
 *
 * @param temporary Indicates if the material resource is only temporary. This allows the
 * resource table to conduct cleanup operations properly.
 */
void SaveMaterialProto(MaterialProto const &material_proto, bool temporary, ResourceTable *table);

} // namespace e8

#endif // ISLANDS_RESOURCE_MATERIAL_H
