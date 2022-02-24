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

#include <google/protobuf/repeated_field.h>
#include <string>

#include "common/tensor.h"
#include "content/proto/primitive.pb.h"

namespace e8 {

/**
 * @brief GenerateUuid Generates a unique ID for every function call. This function is thread-safe.
 */
std::string GenerateUuid();

/**
 * @brief ToProto Converts a 2D vector to a protobuf array.
 */
google::protobuf::RepeatedField<float> ToProto(vec2 const &v);

/**
 * @brief ToVec2 Converts a protobuf array back to a 2D vector. It will check if the array has
 * exactly 2 elements. If not, it will fail.
 */
vec2 ToVec2(google::protobuf::RepeatedField<float> const &proto);

/**
 * @brief ToProto Converts a 3D vector to a protobuf array.
 */
google::protobuf::RepeatedField<float> ToProto(vec3 const &v);

/**
 * @brief ToVec3 Converts a protobuf array back to a 3D vector. It will check if the array has
 * exactly 3 elements. If not, it will fail.
 */
vec3 ToVec3(google::protobuf::RepeatedField<float> const &proto);

/**
 * @brief ToProto Converts a 4 by 4 matrix to a protobuf array. The data is stored in column-major
 * order.
 */
google::protobuf::RepeatedField<float> ToProto(mat44 const &m);

/**
 * @brief ToMat44 Converts a protobuf array back to a 4 by 4 matrix. It will check if the array has
 * exactly 16 elements. If not, it will fail.
 */
mat44 ToMat44(google::protobuf::RepeatedField<float> const &proto);

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

} // namespace e8

#endif // CONTENT_COMMON_H
