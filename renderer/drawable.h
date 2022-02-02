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

#include <optional>
#include <string>
#include <vector>

#include "common/tensor.h"

namespace e8 {

// A 3D coordinate defining the position of a primitive's vertex.
using PrimitiveVertex = vec3;

// Defines a normalized coordinate on a 2D image.
using PrimitiveTextureCoordinate = vec2;

// Defines a triangle by referencing from the PrimitiveVertex list.
using PrimitiveIndices = vec<3, unsigned>;

/**
 * @brief The IslandsDrawableInterface class Represents an object that can be rendered by the
 * renderer. It provides all the information about the object for it to be efficiently processed by
 * the rendering pipeline.
 */
class IslandsDrawableInterface {
  public:
    /**
     * @brief IslandsDrawableInterface The name should be provided for debugging purposes.
     */
    IslandsDrawableInterface(std::string const &human_readable_name);
    ~IslandsDrawableInterface();

    /**
     * @brief HumanReadableName The human readable name of this drawable.
     */
    std::string HumanReadableName() const;

    /**
     * @brief The RigidityType enum The type of rigidity to be expected for this drawable.
     */
    enum RigidityType {
        // The geometry of this drawable can never change.
        RIGID,

        // The vertices of this drawable can be changed at any moment. However, the topology stays
        // the same.
        DEFORMABLE,

        // Even the topology of this drawable can be changed at any moment.
        TEARABLE
    };

    /**
     * @brief Rigidity The type of rigidity to be expected for this drawable.
     */
    virtual RigidityType Rigidity() const = 0;

    /**
     * @brief Transformation The homogeneous transformation to be applied to this drawable's
     * geometry.
     */
    virtual mat44 Transformation() const = 0;

    /**
     * @brief BoundingBox An AABB bounding box surrounding the drawable's geometry.
     */
    virtual aabb BoundingBox() const = 0;

    /**
     * @brief Vertices A list of vertices defining the geomeotry of this drawable.
     */
    virtual std::vector<PrimitiveVertex> const &Vertices() const = 0;

    /**
     * @brief TextureCoordinates This list is optional if the material doesn't require a texture. It
     * defines the normalized 2D coordinates in a texture image for each vertex to make the texture
     * image wrap around the drawable.
     */
    virtual std::vector<PrimitiveTextureCoordinate> const &TextureCoordinates() const = 0;

    /**
     * @brief Indices Defines triangle primitives by referencing the vertices of each triangle from
     * the vertex list returned by the above function.
     */
    virtual std::vector<PrimitiveIndices> const &Indices() const = 0;

  private:
    std::string const human_readable_name_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DRAWABLE_H
