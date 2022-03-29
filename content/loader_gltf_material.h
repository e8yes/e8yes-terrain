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

#ifndef ISLANDS_CONTENT_LOADER_GLTF_MATERIAL_H
#define ISLANDS_CONTENT_LOADER_GLTF_MATERIAL_H

#include "resource/proto/material.pb.h"
#include "third_party/tiny_gltf/tiny_gltf.h"

namespace e8 {

/**
 * @brief LoadMaterial Converts a glTF material to a material protobuf object.
 */
MaterialProto LoadMaterial(tinygltf::Material const &material, tinygltf::Model const &model);

} // namespace e8

#endif // ISLANDS_CONTENT_LOADER_GLTF_MATERIAL_H
