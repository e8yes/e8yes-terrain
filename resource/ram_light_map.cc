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

#include <boost/log/trivial.hpp>
#include <memory>

#include "common/cache.h"
#include "common/device.h"
#include "resource/light_map.h"
#include "resource/proto/table.pb.h"
#include "resource/ram_light_map.h"

namespace e8 {
namespace {

uint64_t ResourceSize(LightMapId const &light_map_id, ResourceTable const &table) {
    return table.light_maps().at(light_map_id).resource_size();
}

void LoadMaterial(LightMapId const &light_map_id, ResourceTable const &table,
                  VulkanContext *context, std::shared_ptr<LightMap> *light_map) {
    BOOST_LOG_TRIVIAL(info) << "LoadMaterial(): ID=[" << light_map_id << "].";

    *light_map = std::make_shared<LightMap>();
    (*light_map)->FromDisk(light_map_id, table, context);
}

} // namespace

LightMapRamTransfer::LightMapRamTransfer(MemoryUsageTracker *tracker) : cache_(tracker) {}

LightMapRamTransfer::~LightMapRamTransfer() {}

std::shared_ptr<LightMap> LightMapRamTransfer::Load(LightMapId const &id,
                                                    ResourceTable const &resource_table,
                                                    VulkanContext *context) {
    std::shared_ptr<LightMap> *upload_result = cache_.Upload(
        id, /*override_old_upload=*/false, /*object_size_fn=*/
        [&resource_table](LightMapId id) { return ResourceSize(id, resource_table); },
        /*upload_fn=*/
        [context, &resource_table](LightMapId id, uint64_t /*old_object_size*/,
                                   uint64_t /*new_object_size*/,
                                   std::shared_ptr<LightMap> *light_map) {
            LoadMaterial(id, resource_table, context, light_map);
        });

    if (upload_result == nullptr) {
        return nullptr;
    }

    return *upload_result;
}

} // namespace e8
