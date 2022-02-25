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

#ifndef ISLANDS_CONTENT_PROCEDURAL_BASIC_H
#define ISLANDS_CONTENT_PROCEDURAL_BASIC_H

#include "common/tensor.h"
#include "content/common.h"
#include "content/procedural_object.h"
#include "content/proto/procedural_object.pb.h"
#include "content/scene_object.h"

namespace e8 {

/**
 * @brief The ProceduralPlane class A one-sided upward facing plane, centered at the origin.
 */
class ProceduralPlane : public ProceduralObjectInterface {
  public:
    /**
     * @brief ProceduralPlane Construct a plane procedural object based on the specified parameters.
     *
     * @param name A descriptive human readable name of the object.
     * @param width How much the plane spans along the x axis?
     * @param height How much the plane spans along the y axis?
     * @param cell_area It controls the subdivision. The subdivision process divides the plane into
     * quads. This field specifies the maximum area the quad is allowed to be.
     * @param srt_transform Manual transformation input.
     */
    ProceduralPlane(ProceduralObjectName const &name, float width, float height, float cell_area,
                    SrtTransform const &srt_transform);

    /**
     * @brief ProceduralPlane Constructs a plane procedural object with parameters provided by a
     * proto object.
     */
    explicit ProceduralPlane(ProceduralObjectProto const &proto);

    ~ProceduralPlane();

    SceneObject ToSceneObject() const override;
    ProceduralObjectProto ToProto() const override;

  public:
    // How much the plane spans along the x axis?
    float width;

    // How much the plane spans along the y axis?
    float height;

    // It controls the subdivision. The subdivision process divides the plane into quads. This field
    // specifies the maximum area the quad is allowed to be.
    float cell_area;

    // Manual transformation input.
    SrtTransform srt_transform;
};

} // namespace e8

#endif // ISLANDS_CONTENT_PROCEDURAL_BASIC_H
