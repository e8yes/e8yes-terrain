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
#include "resource/buffer_image.h"
#include "resource/common.h"
#include "resource/proto/material.pb.h"
#include "resource/proto/table.pb.h"

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

    /**
     * @brief ToDisk It saves the material data to disk and populates the material resource's
     * metadata to the specified resource table.
     *
     * @param temporary Indicates if the material resource is only temporary. This allows the
     * resource table to conduct cleanup operations properly.
     */
    void ToDisk(bool temporary, ResourceTable *table);

    // ID of this material design.
    MaterialId id;

    // A descriptive human readable name of this material.
    MaterialName name;

    // The albedo map.
    StagingImageBuffer albedo;

    // The normal map.
    StagingImageBuffer normal;

    // The metallic map.
    StagingImageBuffer metallic;

    // The roughness map.
    StagingImageBuffer roughness;
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
