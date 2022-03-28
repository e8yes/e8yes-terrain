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

#ifndef ISLANDS_RESOURCE_LIGHT_MAP_H
#define ISLANDS_RESOURCE_LIGHT_MAP_H

#include "common/device.h"
#include "resource/buffer_image.h"
#include "resource/common.h"
#include "resource/proto/light_map.pb.h"
#include "resource/proto/table.pb.h"

namespace e8 {

// It uniquely identifies a light map.
using LightMapId = Uuid;

/**
 * @brief The LightMap struct It stores pre-computed radiosity for static lights and surfaces.
 */
struct LightMap {
    LightMap();
    LightMap(LightMap const &) = delete;
    LightMap(LightMap &&) = default;
    ~LightMap();

    /**
     * @brief FromDisk Recovers data from disk. If the data isn't available, this function will
     * fail. It uses the specified ID to look up resource metadata from the resource table, then it
     * recovers the material from data on disk to Vulkan buffer memory associated with the Vulkan
     * context.
     */
    void FromDisk(LightMapId const &id, ResourceTable const &table, VulkanContext *context);

    /**
     * @brief ToDisk It saves the light map data to disk and populates the light map resource's
     * metadata to the specified resource table.
     *
     * @param temporary Indicates if the light map resource is only temporary. This allows the
     * resource table to conduct cleanup operations properly.
     */
    void ToDisk(bool temporary, ResourceTable *table);

    // ID of this light map.
    LightMapId id;

    // It stores the radiosity map contributed from some or all light bounces.
    StagingImageBuffer radiosity;
};

/**
 * @brief SaveLightMapProto Saves the specified light map proto to disk and populates the light map
 * resource's metadata to the specified resource table.
 *
 * @param temporary Indicates if the light map resource is only temporary. This allows the
 * resource table to conduct cleanup operations properly.
 */
void SaveLightMapProto(LightMapProto const &light_map_proto, bool temporary, ResourceTable *table);

} // namespace e8

#endif // ISLANDS_RESOURCE_LIGHT_MAP_H
