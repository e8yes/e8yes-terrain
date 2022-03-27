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

#ifndef ISLANDS_RESOURCE_RAM_MATERIAL_H
#define ISLANDS_RESOURCE_RAM_MATERIAL_H

#include <memory>

#include "common/cache.h"
#include "common/device.h"
#include "resource/material.h"
#include "resource/proto/table.pb.h"

namespace e8 {

/**
 * @brief The MaterialRamTransfer class It encapsulate an efficient material data transfer from disk
 * to RAM via caching. This class isn't thread-safe.
 */
class MaterialRamTransfer {
  public:
    /**
     * @brief MaterialRamTransfer The tracker's capacity will put a limit to the cache size.
     */
    MaterialRamTransfer(MemoryUsageTracker *tracker);
    ~MaterialRamTransfer();

    /**
     * @brief Load Loads a material from disk, pointed to by its ID and its metadata in the resource
     * table, if it hasn't been in the cache.
     */
    std::shared_ptr<Material> Load(MaterialId const &id, ResourceTable const &resource_table,
                                   VulkanContext *context);

  private:
    DeviceCache<MaterialId, std::shared_ptr<Material>> cache_;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_RAM_MATERIAL_H
