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

#ifndef ISLANDS_RENDERER_DRAWABLE_H
#define ISLANDS_RENDERER_DRAWABLE_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/tensor.h"

namespace e8 {

// It uniquely identifies a drawable design with its LOD series.
using DrawableId = std::string;

/**
 * @brief The PrimitiveVertex struct It defines the geometric attributes of a primitive's vertex.
 */
struct PrimitiveVertex {
    // Defines the vertex's 3D position.
    vec3 position;

    // Defines the normal vector for the vertex position.
    vec3 normal;

    // Defines the normalized 2D coordinates of a texture image to make it wrap around the surface
    // surrounding this vertex.
    vec2 texcoord;
};

// Defines a triangle with vertices referencing from the PrimitiveVertex list.
using PrimitiveIndices = vec<3, unsigned>;

/**
 * @brief The Drawable struct Represents an object that can be rendered by the renderer. It provides
 * all the information about the object for it to be efficiently processed by the rendering
 * pipeline.
 */
struct Drawable {
    // The human readable name of this drawable, provided for debugging purposes.
    std::string human_readable_name;

    /**
     * @brief The RigidityType enum The type of rigidity to be expected for this drawable.
     */
    enum RigidityType {
        // This drawable's geometry can neither be moved nor changed.
        STATIC,

        // The geometry of this drawable can never change though it can be moved around through
        // homogeneous transformation.
        RIGID,

        // The vertices of this drawable can be changed at any moment. However, the topology stays
        // the same.
        DEFORMABLE,

        // Even the topology of this drawable can be changed at any moment.
        TEARABLE
    };

    // The type of rigidity to be expected for this drawable.
    RigidityType rigidity;

    // A list of vertices defining the geometry of this drawable.
    std::vector<PrimitiveVertex> vertices;

    // Defines triangle primitives by referencing the vertices of each triangle from the vertex list
    // returned by the above function.
    std::vector<PrimitiveIndices> indices;
};

/**
 * @brief The DrawableLod struct Allows complex drawable to be represented with less detail at
 * distance.
 */
struct DrawableLod {
    // ID of this drawable design.
    DrawableId id;

    // An AABB bounding box surrounding the drawable's geometry.
    aabb bounding_box;

    // LOD of this drawable design in descending level of detail.
    std::vector<Drawable> drawable_lod;

    // A list the same length as the drawable_lod specifying the minimum distance the drawble LOD is
    // from the viewer before it can be applied to the rendering process. Therefore, the distances
    // are expected to be in ascending order. The renderer will pick the drawable with the least
    // possible detail when possible.
    std::vector<float> min_distances;
};

/**
 * @brief The DrawableLodInstance struct An object with information derived from a drawable. This is
 * what actually gets rendered. It allows a large number of same objects to be placed at different
 * location of the scene with sharing the same information.
 */
struct DrawableLodInstance {
    // The drawable information the instance is derived from.
    std::shared_ptr<DrawableLod> drawable;

    // The homogeneous transformation to be applied to the drawable's geometry.
    mat44 transform;
};

/**
 * @brief The DrawableInstance struct Stores references to a DrawableLodInstance.
 */
struct DrawableInstance {
    // The drawable selected from a series of LODs.
    Drawable const *drawable;

    // The homogeneous transformation to be applied to the drawable's geometry.
    mat44 const *transform;
};

// Uniquely identifies a scene object.
using SceneObjectId = std::string;

/**
 * @brief The SceneObject struct It logically groups a set of drawable instances.
 */
struct SceneObject {
    // ID of this scene object.
    SceneObjectId id;

    // The drawable group that constitutes this scene object.
    std::vector<std::unique_ptr<DrawableLodInstance>> drawables;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DRAWABLE_H
