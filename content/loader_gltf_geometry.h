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

#ifndef ISLANDS_CONTENT_LOADER_GLTF_GEOMETRY_H
#define ISLANDS_CONTENT_LOADER_GLTF_GEOMETRY_H

#include <optional>
#include <string>

#include "resource/proto/geometry.pb.h"
#include "third_party/tiny_gltf/tiny_gltf.h"

namespace e8 {

/**
 * @brief LoadGeometry Converts a glTF primitive to a geometry protobuf object. In order for the
 * conversion be successful, the primitive must contains these vertex attributes: position and at
 * least one set of texture coordinate. Otherwise, this function returns a nullopt. Besides, the
 * primitive must be modeled as a triangle mesh.
 */
std::optional<GeometryProto> LoadGeometry(tinygltf::Primitive const &primitive,
                                          std::string const &name, tinygltf::Model const &model);

} // namespace e8

#endif // ISLANDS_CONTENT_LOADER_GLTF_GEOMETRY_H
