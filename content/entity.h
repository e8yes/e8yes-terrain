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
#include <string>
#include <unordered_map>
#include <vector>

#include "common/tensor.h"
#include "content/drawable.h"
#include "content/proto/drawable.pb.h"
#include "content/proto/entity.pb.h"
#include "content/proto/physical_shape.pb.h"

namespace e8 {

// Uniquely identifies a scene entity.
using SceneEntityId = std::string;

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
     * @brief SceneEntity Constructs an empty entity, with an automatically assigned unique ID.
     */
    SceneEntity();

    /**
     * @brief SceneEntity Constructs an entity from proto message. Since the proto references the
     * drawable and physical shape information by only IDs, it requires two maps of actually
     * instances to correctly fill the entity up.
     */
    SceneEntity(SceneEntityProto const &proto,
                std::unordered_map<DrawableId, std::shared_ptr<DrawableLod>> const &drawables);

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

    // An AABB bounding box surrounding the entity's geometry.
    aabb bounding_box;

    // A drawable instance with information derived from a drawable. A shared pointer allows a large
    // number of same drawables to be placed at different location of the scene with shared
    // information.
    std::shared_ptr<DrawableLod> drawable_lod_instance;

    // TODO: Adds physical shape instance once it's implemented.
};

/**
 * @brief ToProto Transforms a collection of scene entities to a SceneEntityCollection message. See
 * the proto definition for what it is.
 */
SceneEntityCollection ToProto(std::vector<SceneEntity> const &entities);

/**
 * @brief ToSceneEntities Transforms a proto description of a collection of entities back to an
 * in-memory SceneEntity collection.
 */
std::vector<SceneEntity> ToSceneEntities(SceneEntityCollection const &proto);

} // namespace e8

#endif // ISLANDS_CONTENT_ENTITY_H
