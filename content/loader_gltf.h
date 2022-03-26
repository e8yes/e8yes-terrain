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

#ifndef ISLANDS_RESOURCE_LOADER_GLTF_H
#define ISLANDS_RESOURCE_LOADER_GLTF_H

#include <string>
#include <vector>

#include "common/device.h"
#include "content/scene_object.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief LoadFromGltf Loads scene objects from a glTF file.
 *
 * @param gltf_file_path The file to be loaded.
 * @param resource_accessor Where glTF resources will be imported.
 * @return A list of root objects specified in the glTF file.
 */
std::vector<SceneObject> LoadFromGltf(std::string const &gltf_file_path,
                                      ResourceAccessor *resource_accessor);

} // namespace e8

#endif // ISLANDS_RESOURCE_LOADER_GLTF_H
