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
#include "resource/proto/geometry.pb.h"
#include "resource/proto/texture.pb.h"

namespace e8 {

// Uniquely identifies a procedural object.
using ProceduralObjectId = Uuid;

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

/**
 * @brief BoundingBoxOf Computes a bounding box that contains the specified geometry.
 */
aabb BoundingBoxOf(GeometryProto const &geometry);

/**
 * @brief OneByOneAlbedoTexture Creates an albedo texture with exactly one pixel carrying the
 * specified albedo value.
 */
TextureProto OneByOneAlbedoTexture(vec3 const &albedo);

/**
 * @brief OneByOneNormalTexture Creates a normal texture with exactly one pixel carrying a tangent
 * space normal vector pointing up towards the z axis.
 */
TextureProto OneByOneNormalTexture();

/**
 * @brief OneByOneRoughnessTexture Creates an roughness texture with exactly one pixel carrying the
 * specified roughness value.
 */
TextureProto OneByOneRoughnessTexture(float roughness);

/**
 * @brief OneByOneMetallicTexture Creates an metallic texture with exactly one pixel carrying the
 * specified metallic value.
 */
TextureProto OneByOneMetallicTexture(float metallic);

} // namespace e8

#endif // CONTENT_COMMON_H
