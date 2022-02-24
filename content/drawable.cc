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
#include "content/drawable.h"
#include "content/proto/drawable.pb.h"

namespace e8 {

std::shared_ptr<DrawableLod>
CreateDrawable(DrawableName const &name, google::protobuf::RepeatedPtrField<Drawable> const &lod,
               google::protobuf::RepeatedField<float> const &lod_min_distances) {
    auto drawable_lod = std::make_unique<DrawableLod>();
    drawable_lod->set_id(GenerateUuid());
    drawable_lod->set_human_readable_name(name);
    *drawable_lod->mutable_drawable_lod() = lod;
    *drawable_lod->mutable_min_distances() = lod_min_distances;
    return drawable_lod;
}

} // namespace e8
