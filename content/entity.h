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

#include "common/tensor.h"
#include "content/drawable.h"

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
    // ID of this scene entity.
    SceneEntityId id;

    // Human readable name of this scene entity.
    SceneEntityName name;

    // Indicates if the entity is movable.
    bool movable;

    // The homogeneous transformation to be applied to this entity's geometry.
    mat44 transform;

    // A drawable instance with information derived from a drawable. A shared pointer allows a large
    // number of same drawables to be placed at different location of the scene with shared
    // information.
    std::shared_ptr<DrawableLod> drawable_lod_instance;
};

// Uniquely identifies a scene object.
using SceneObjectId = std::string;

// Represents a descriptive human readable name of a scene object.
using SceneObjectName = std::string;

/**
 * @brief The SceneObject struct It logically groups a set of entities.
 */
struct SceneObject {
    // ID of this scene object.
    SceneObjectId id;

    // Human readable name of this scene object.
    SceneObjectName name;

    // The entity group that constitutes this scene object.
    std::vector<SceneObjectId> entities;
};

} // namespace e8

#endif // ISLANDS_CONTENT_ENTITY_H
