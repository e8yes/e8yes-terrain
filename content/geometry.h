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

#ifndef ISLANDS_CONTENT_GEOMETRY_H
#define ISLANDS_CONTENT_GEOMETRY_H

#include <google/protobuf/repeated_field.h>
#include <memory>
#include <string>

#include "content/proto/geometry.pb.h"

namespace e8 {

// It uniquely identifies a geometry design with its LOD series.
using GeometryId = std::string;

// A descriptive human readable name of a geometry LOD series.
using GeometryName = std::string;

/**
 * @brief CreateGeometry Creates a geometry with LOD. All GeometryLods should be created here.
 *
 * @param name A descriptive human readable name for the geometry.
 * @param lod A series of geometry LOD in decreasing detail.
 * @param lod_min_distances The minimum distance from the viewer the ith LOD can be used.
 * @return A geometry with LOD.
 */
std::shared_ptr<GeometryLod>
CreateGeometry(GeometryName const &name, google::protobuf::RepeatedPtrField<Geometry> const &lod,
               google::protobuf::RepeatedField<float> const &lod_min_distances);

} // namespace e8

#endif // ISLANDS_CONTENT_GEOMETRY_H
