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

#ifndef ISLANDS_RESOURCE_TABLE_H
#define ISLANDS_RESOURCE_TABLE_H

#include <filesystem>
#include <memory>

#include "resource/proto/table.pb.h"

namespace e8 {

/**
 * @brief CreateResourceTable Creates a new resource table on the specified base_path.
 */
std::unique_ptr<ResourceTable> CreateResourceTable(std::filesystem::path const &base_path);

/**
 * @brief LoadResourceTable Loads an existing resource table from the specified base_path.
 */
std::unique_ptr<ResourceTable> LoadResourceTable(std::filesystem::path const &base_path);

/**
 * @brief SaveResourceTable Saves the resource table to disk, under the table's current base path.
 */
bool SaveResourceTable(ResourceTable const &table);

/**
 * @brief ResourceTableRemoveOrphanedFiles Removes all resource files from the transient table that
 * don't exist in the persisted table.
 */
void ResourceTableRemoveOrphanedFiles(ResourceTable const &persisted_table,
                                      ResourceTable *transient_table);

/**
 * @brief ResourceTableRemoveTemporaryFiles Removes all temporary resource files from the table.
 */
void ResourceTableRemoveTemporaryFiles(ResourceTable *table);

} // namespace e8

#endif // ISLANDS_RESOURCE_TABLE_H
