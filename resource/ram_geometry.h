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

#ifndef ISLANDS_RESOURCE_RAM_GEOMETRY_H
#define ISLANDS_RESOURCE_RAM_GEOMETRY_H

#include "common/cache.h"
#include "common/device.h"
#include "resource/geometry.h"
#include "resource/proto/table.pb.h"

namespace e8 {

/**
 * @brief The GeometryRamTransfer class It encapsulate an efficient geometry data transfer from disk
 * to RAM via caching. This class isn't thread-safe.
 */
class GeometryRamTransfer {
  public:
    /**
     * @brief GeometryRamTransfer The tracker's capacity will put a limit to the cache size.
     */
    GeometryRamTransfer(MemoryUsageTracker *tracker);
    ~GeometryRamTransfer();

    /**
     * @brief Load Loads a geometry pointed to by its ID and metadata in the resource table from
     * disk if it hasn't been in the cache.
     */
    Geometry *Load(GeometryId const &id, ResourceTable const &resource_table,
                   VulkanContext *context);

  private:
    DeviceCache<GeometryId, Geometry> cache_;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_RAM_GEOMETRY_H
