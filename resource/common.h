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

#ifndef ISLANDS_RESOURCE_COMMON_H
#define ISLANDS_RESOURCE_COMMON_H

#include <cstdint>
#include <google/protobuf/repeated_field.h>

#include "common/tensor.h"
#include "resource/proto/primitive.pb.h"
#include "resource/proto/texture.pb.h"

namespace e8 {

// Unique IDs to assign to each resource.
using Uuid = uint64_t;

// An ID reserved for representing null values.
Uuid const kNullUuid = 0;

/**
 * @brief GenerateUuid Generates a unique ID for every function call. This function is thread-safe.
 */
Uuid GenerateUuid();

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
 * @brief ToProto Converts a 4D vector to a protobuf array.
 */
google::protobuf::RepeatedField<float> ToProto(vec4 const &v);

/**
 * @brief ToVec4 Converts a protobuf array back to a 4D vector. It will check if the array has
 * exactly 4 elements. If not, it will fail.
 */
vec4 ToVec4(google::protobuf::RepeatedField<float> const &proto);

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
 * @brief EncodeTextureData Encodes the bitmap into the texture protobuf object's data field.
 *
 * @param bitmap Points to the memory region that stores the bitmap.
 * @param texture_proto In order for the bitmap to be correctly encoded, this protobuf object must
 * have everything except the data field correctly specified.
 */
void EncodeTextureData(void const *bitmap, TextureProto *texture_proto);

} // namespace e8

#endif // ISLANDS_RESOURCE_COMMON_H
