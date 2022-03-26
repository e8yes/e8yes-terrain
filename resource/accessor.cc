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

#include <cassert>
#include <filesystem>
#include <memory>
#include <vector>

#include "common/device.h"
#include "resource/accessor.h"
#include "resource/geometry.h"
#include "resource/proto/geometry.pb.h"
#include "resource/proto/table.pb.h"
#include "resource/ram_geometry.h"
#include "resource/table.h"

namespace e8 {

ResourceAccessor::ResourceAccessor(std::filesystem::path const &base_path, bool load_existing,
                                   VulkanContext *device)
    : device_(device) {
    if (load_existing) {
        persisted_table_ = LoadResourceTable(base_path);
        if (persisted_table_ == nullptr) {
            return;
        }

        ResourceTableRemoveTemporaryFiles(persisted_table_.get());
    } else {
        persisted_table_ = CreateResourceTable(base_path);
    }

    transient_table_ = std::make_unique<ResourceTable>(*persisted_table_);

    geometry_ram_transfer_ = std::make_unique<GeometryRamTransfer>(&device->geometry_ram_usage);
}

ResourceAccessor::~ResourceAccessor() {
    if (!this->Valid()) {
        return;
    }

    ResourceTableRemoveTemporaryFiles(transient_table_.get());
    ResourceTableRemoveOrphanedFiles(*persisted_table_, transient_table_.get());
}

bool ResourceAccessor::Valid() const { return transient_table_ != nullptr; }

void ResourceAccessor::AddGeometry(GeometryProto const &proto, bool temporary) {
    SaveGeometryProto(proto, temporary, transient_table_.get());
}

std::shared_ptr<Geometry> ResourceAccessor::LoadGeometry(GeometryId const &id) {
    return geometry_ram_transfer_->Load(id, *transient_table_, device_);
}

void ResourceAccessor::RemoveGeometry(GeometryId const &id) {
    transient_table_->mutable_geometries()->erase(id);
}

bool ResourceAccessor::Commit() {
    if (!SaveResourceTable(*transient_table_)) {
        return false;
    }

    *persisted_table_ = *transient_table_;
    return true;
}

} // namespace e8
