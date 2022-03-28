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

#ifndef ISLANDS_RESOURCE_ACCESSOR_H
#define ISLANDS_RESOURCE_ACCESSOR_H

#include <filesystem>
#include <memory>
#include <vector>

#include "common/device.h"
#include "resource/geometry.h"
#include "resource/light_map.h"
#include "resource/material.h"
#include "resource/proto/geometry.pb.h"
#include "resource/proto/material.pb.h"
#include "resource/proto/table.pb.h"
#include "resource/ram_geometry.h"
#include "resource/ram_light_map.h"
#include "resource/ram_material.h"

namespace e8 {

/**
 * @brief The ResourceAccessor class This is the portable to this resource package. Load/store
 * operations to geometry, material, light map and physical shape resources should generally be done
 * through this class.
 */
class ResourceAccessor {
  public:
    /**
     * @brief ResourceAccessor Constructs a resource accessor on the specified base_path.
     *
     * @param base_path The "root" directory of all resources.
     * @param load_existing Whether to load resource metadata from the base path. If not, an empty
     * resource accessor is created. Use the ResourceAccessor::Valid() call to check if the load is
     * successful.
     * @param device Contextual Vulkan handles for resource creation.
     */
    ResourceAccessor(std::filesystem::path const &base_path, bool load_existing,
                     VulkanContext *device);
    ResourceAccessor(ResourceAccessor const &) = delete;
    ResourceAccessor(ResourceAccessor &&) = default;
    ~ResourceAccessor();

    /**
     * @brief Valid Checks if the accessor was loaded successfully from the base path. Note, a new
     * creation though is always valid.
     */
    bool Valid() const;

    /**
     * @brief AddGeometry Adds a new geometry resource.
     *
     * @param proto The protobuf object that defines the geometry data.
     * @param temporary Indicates if the resource is only temporary. A temporary resource will get
     * cleaned up by the end of this object's lifecycle.
     */
    void AddGeometry(GeometryProto const &proto, bool temporary);

    /**
     * @brief LoadGeometry Loads an existing geometry pointed by the specifed ID. If the load fails,
     * this function will also fail.
     */
    std::shared_ptr<Geometry> LoadGeometry(GeometryId const &id);

    /**
     * @brief RemoveGeometry Removes the geometry pointed to by the ID. If such geometry doesn't
     * exist, it does nothing.
     */
    void RemoveGeometry(GeometryId const &id);

    /**
     * @brief AddGeometry Adds a new material resource.
     *
     * @param proto The protobuf object that defines the geometry data.
     * @param temporary Indicates if the resource is only temporary. A temporary resource will get
     * cleaned up by the end of this object's lifecycle.
     */
    void AddMaterial(MaterialProto const &proto, bool temporary);

    /**
     * @brief LoadMaterial Loads an existing material pointed by the specifed ID. If the load fails,
     * this function will also fail.
     */
    std::shared_ptr<Material> LoadMaterial(MaterialId const &id);

    /**
     * @brief RemoveGeometry Removes the material pointed to by the ID. If such material doesn't
     * exist, it does nothing.
     */
    void RemoveMaterial(MaterialId const &id);

    /**
     * @brief LoadLightMap Loads an existing light map pointed by the specifed ID. If the load
     * fails, this function will also fail.
     */
    std::shared_ptr<LightMap> LoadLightMap(LightMapId const &id);

    /**
     * @brief RemoveLightMap Removes the light map pointed to by the ID. If such light map doesn't
     * exist, it does nothing.
     */
    void RemoveLightMap(LightMapId const &id);

    /**
     * @brief Commit Saves transient changes to the disk.
     */
    bool Commit();

  private:
    VulkanContext *device_;
    std::unique_ptr<ResourceTable> transient_table_;
    std::unique_ptr<ResourceTable> persisted_table_;
    std::unique_ptr<GeometryRamTransfer> geometry_ram_transfer_;
    std::unique_ptr<MaterialRamTransfer> material_ram_transfer_;
    std::unique_ptr<LightMapRamTransfer> light_map_ram_transfer_;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_ACCESSOR_H
