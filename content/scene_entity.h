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

#ifndef ISLANDS_CONTENT_ENTITY_H
#define ISLANDS_CONTENT_ENTITY_H

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/tensor.h"
#include "content/proto/entity.pb.h"
#include "content/proto/light_source.pb.h"
#include "content/proto/lod.pb.h"
#include "resource/common.h"
#include "resource/geometry.h"
#include "resource/proto/geometry.pb.h"
#include "resource/proto/physical_shape.pb.h"

namespace e8 {

// Uniquely identifies a scene entity.
using SceneEntityId = Uuid;

// Represents a descriptive human readable name of a scene entity.
using SceneEntityName = std::string;

/**
 * @brief The SceneEntity struct A scene entity is the smallest unit of information holder that
 * describes
 * 1. how it is displayed;
 * 2. how it animates;
 * 3. how it interacts with other scene entities;
 * 4. how it acts;
 * 5. and how it signals an observer function.
 */
struct SceneEntity {
    /**
     * @brief SceneEntity Constructs an empty movable entity, with an automatically assigned unique
     * ID.
     *
     * @param A descriptive human readable name for this entity.
     */
    SceneEntity(SceneEntityName const &name);

    /**
     * @brief SceneEntity Constructs an entity from proto message.
     */
    SceneEntity(SceneEntityProto const &proto);

    ~SceneEntity();

    /**
     * @brief ToProto Returns a proto description of the entity.
     */
    SceneEntityProto ToProto() const;

    // ID of this scene entity.
    SceneEntityId id;

    // Human readable name of this scene entity.
    SceneEntityName name;

    // Indicates if the entity is movable.
    bool movable;

    // The homogeneous transformation to be applied to this entity's geometry.
    mat44 transform;

    // Most likely inputs by a human. It may not always be available because a general homogeneous
    // transform cannot be reverted back to an SRT specification.
    std::optional<SrtTransform> srt_transform;

    // An AABB bounding box surrounding the entity's geometry prior to any transformation.
    aabb bounding_box;

    // A list of LOD references stored in decreasing level of detail.
    SceneEntityResources resources;

    // Light source attached to the entity.
    std::optional<LightSource> light_source;
};

/**
 * @brief SceneEntitySetSrtTransform Sets an SRT transform to the entity. It will synchronize the
 * homogeneous transform with it.
 */
void SceneEntitySetSrtTransform(SrtTransform const &srt_transform, SceneEntity *entity);

/**
 * @brief SceneEntitySetTransform Sets a homogeneous transform to the entity. It will clear out the
 * SRT transform if there is one. An SRT can't be set because SRT information can't be extracted
 * from general homogeneous transform.
 */
void SceneEntitySetTransform(mat44 const &transform, SceneEntity *entity);

} // namespace e8

#endif // ISLANDS_CONTENT_ENTITY_H
