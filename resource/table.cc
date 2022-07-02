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
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <google/protobuf/map.h>
#include <memory>

#include "resource/proto/table.pb.h"
#include "resource/table.h"

namespace e8 {
namespace {

constexpr char const *kResourceTableFileName = "table.tpb";

std::filesystem::path ResourceTablePath(std::filesystem::path const &base_path) {
    return base_path / kResourceTableFileName;
}

std::filesystem::path ResourceFilePath(std::filesystem::path const &base_path,
                                       std::filesystem::path const &resource_file_name) {
    return base_path / resource_file_name;
}

void RemoveTemporaryFiles(std::filesystem::path const &base_path,
                          google::protobuf::Map<int64_t, ResourceTable::Metadata> *items) {
    std::vector<int64_t> to_be_removed;

    for (auto const &[key, metadata] : *items) {
        if (!metadata.temporary()) {
            continue;
        }

        to_be_removed.push_back(key);

        std::filesystem::path file_path = ResourceFilePath(base_path, metadata.file_name());
        if (0 != remove(file_path)) {
            BOOST_LOG_TRIVIAL(info) << "RemoveTemporaryFiles(): Temporary file=[" << file_path
                                    << "] can't be removed. errno=[" << errno << "].";
        }
    }

    for (auto const &item : to_be_removed) {
        items->erase(item);
    }
}

void RemoveOrphanedFiles(
    std::filesystem::path const &base_path,
    google::protobuf::Map<int64_t, ResourceTable::Metadata> const &persisted_items,
    google::protobuf::Map<int64_t, ResourceTable::Metadata> *transient_items) {
    std::vector<int64_t> to_be_removed;

    for (auto const &[key, metadata] : *transient_items) {
        auto it = persisted_items.find(key);
        if (it != persisted_items.end()) {
            // Not orphaned.
            continue;
        }

        to_be_removed.push_back(key);

        std::filesystem::path file_path = ResourceFilePath(base_path, metadata.file_name());
        if (0 != remove(file_path)) {
            BOOST_LOG_TRIVIAL(info) << "RemoveOrphanedFiles(): Orphaned file=[" << file_path
                                    << "] can't be removed. errno=[" << errno << "].";
        }
    }

    for (auto const &item : to_be_removed) {
        transient_items->erase(item);
    }
}

} // namespace

std::unique_ptr<ResourceTable> CreateResourceTable(std::filesystem::path const &base_path) {
    auto table = std::make_unique<ResourceTable>();
    table->set_current_base_path(base_path.string());
    return table;
}

std::unique_ptr<ResourceTable> LoadResourceTable(std::filesystem::path const &base_path) {
    std::filesystem::path file_path = ResourceTablePath(base_path);
    std::fstream table_file(file_path, std::ios::in | std::ios::binary);
    if (!table_file.is_open()) {
        return nullptr;
    }

    auto table = std::make_unique<ResourceTable>();
    if (!table->ParseFromIstream(&table_file)) {
        return nullptr;
    }

    table->set_current_base_path(base_path.string());

    return table;
}

bool SaveResourceTable(ResourceTable const &table) {
    std::filesystem::path file_path = ResourceTablePath(table.current_base_path());
    std::fstream table_file(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!table_file.is_open()) {
        return false;
    }

    if (!table.SerializeToOstream(&table_file)) {
        return false;
    }

    return true;
}

void ResourceTableRemoveOrphanedFiles(ResourceTable const &persisted_table,
                                      ResourceTable *transient_table) {
    assert(persisted_table.current_base_path() == transient_table->current_base_path());

    RemoveOrphanedFiles(persisted_table.current_base_path(), persisted_table.geometries(),
                        transient_table->mutable_geometries());
    RemoveOrphanedFiles(persisted_table.current_base_path(), persisted_table.materials(),
                        transient_table->mutable_materials());
    RemoveOrphanedFiles(persisted_table.current_base_path(), persisted_table.light_maps(),
                        transient_table->mutable_light_maps());
    RemoveOrphanedFiles(persisted_table.current_base_path(), persisted_table.physical_shapes(),
                        transient_table->mutable_physical_shapes());
}

void ResourceTableRemoveTemporaryFiles(ResourceTable *table) {
    RemoveTemporaryFiles(table->current_base_path(), table->mutable_geometries());
    RemoveTemporaryFiles(table->current_base_path(), table->mutable_materials());
    RemoveTemporaryFiles(table->current_base_path(), table->mutable_light_maps());
    RemoveTemporaryFiles(table->current_base_path(), table->mutable_physical_shapes());
}

} // namespace e8
