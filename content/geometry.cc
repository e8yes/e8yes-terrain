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

#include <google/protobuf/repeated_field.h>
#include <memory>
#include <string>
#include <vector>

#include "content/common.h"
#include "content/geometry.h"
#include "content/proto/geometry.pb.h"

namespace e8 {

std::shared_ptr<GeometryLod>
CreateGeometry(GeometryName const &name, google::protobuf::RepeatedPtrField<Geometry> const &lod,
               google::protobuf::RepeatedField<float> const &lod_min_distances) {
    auto geometry_lod = std::make_unique<GeometryLod>();
    geometry_lod->set_id(GenerateUuid());
    geometry_lod->set_name(name);
    *geometry_lod->mutable_geometry_lod() = lod;
    *geometry_lod->mutable_min_distances() = lod_min_distances;
    return geometry_lod;
}

} // namespace e8
