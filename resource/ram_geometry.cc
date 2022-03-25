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

#include <memory>

#include "common/cache.h"
#include "common/device.h"
#include "resource/geometry.h"
#include "resource/proto/table.pb.h"
#include "resource/ram_geometry.h"

namespace e8 {
namespace {

uint64_t ResourceSize(GeometryId const &geometry_id, ResourceTable const &table) {
    return table.geometries().at(geometry_id).resource_size();
}

void LoadGeometry(GeometryId const &geometry_id, ResourceTable const &table, VulkanContext *context,
                  std::shared_ptr<GeometryLod> *geometry_lod) {
    *geometry_lod = std::make_shared<GeometryLod>();
    (*geometry_lod)->FromDisk(geometry_id, table, context);
}

} // namespace

GeometryRamTransfer::GeometryRamTransfer(MemoryUsageTracker *tracker) : cache_(tracker) {}

GeometryRamTransfer::~GeometryRamTransfer() {}

std::shared_ptr<GeometryLod> GeometryRamTransfer::Load(GeometryId const &id,
                                                       ResourceTable const &resource_table,
                                                       VulkanContext *context) {
    std::shared_ptr<GeometryLod> *upload_result = cache_.Upload(
        id, /*override_old_upload=*/false, /*object_size_fn=*/
        [&resource_table](GeometryId id) { return ResourceSize(id, resource_table); },
        /*upload_fn=*/
        [context, &resource_table](GeometryId id, uint64_t /*old_object_size*/,
                                   uint64_t /*new_object_size*/,
                                   std::shared_ptr<GeometryLod> *geometry_lod) {
            LoadGeometry(id, resource_table, context, geometry_lod);
        });

    if (upload_result == nullptr) {
        return nullptr;
    }

    return *upload_result;
}

} // namespace e8
