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

#ifndef CONTENT_COMMON_H
#define CONTENT_COMMON_H

#include <string>

#include "common/tensor.h"
#include "content/proto/bbox.pb.h"
#include "content/proto/transform.pb.h"
#include "resource/common.h"

namespace e8 {

/**
 * @brief ToProto Converts an AABB bounding box to a protobuf object.
 */
AABB ToProto(aabb const &a);

/**
 * @brief ToAabb Converts a protobuf object back to an AABB bounding box.
 */
aabb ToAabb(AABB const &proto);

/**
 * @brief ToHomogeneousTransform Expresses an SRT transform as a general homogeneous transform.
 */
mat44 ToHomogeneousTransform(SrtTransform const &srt_transform);

// Uniquely identifies a procedural object.
using ProceduralObjectId = Uuid;

} // namespace e8

#endif // CONTENT_COMMON_H
