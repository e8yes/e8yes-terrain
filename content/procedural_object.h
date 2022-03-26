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

#ifndef ISLANDS_CONTENT_PROCEDURAL_OBJECT_H
#define ISLANDS_CONTENT_PROCEDURAL_OBJECT_H

#include <google/protobuf/repeated_field.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/device.h"
#include "content/common.h"
#include "content/proto/procedural_object.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"
#include "resource/accessor.h"

namespace e8 {

// A descriptive human readable name of a procedural object.
using ProceduralObjectName = std::string;

/**
 * @brief The ProceduralObjectInterface class A general interface for procedural object to integrate
 * with convention scene content. A procedural object usually has geometry defined by parameters
 * other than primitive meshes and appearance given by data other than texture images.
 */
class ProceduralObjectInterface {
  public:
    /**
     * @brief ProceduralObjectInterface Constructs a procedural object and sets its name.
     */
    explicit ProceduralObjectInterface(ProceduralObjectName const &name);

    /**
     * @brief ProceduralObjectInterface Constructs a procedural object base class with content
     * provided by the proto object.
     */
    explicit ProceduralObjectInterface(ProceduralObjectProto const &proto);

    ~ProceduralObjectInterface();

    /**
     * @brief ToSceneObject Converts a procedural object to a regular scene object.
     */
    virtual SceneObject ToSceneObject(ResourceAccessor *resource_accessor) const = 0;

    /**
     * @brief ToProto Converts a procedural object to a serializable protobuf object.
     */
    virtual ProceduralObjectProto ToProto() const = 0;

  public:
    // ID of the object.
    ProceduralObjectId id;

    // Name of the object.
    ProceduralObjectName name;

    // Indicates if the object is movable.
    bool movable;

  protected:
    ProceduralObjectProto
    _ToBaseProto(ProceduralObjectProto::ProceduralObjectType const &object_type) const;
};

/**
 * @brief ToProceduralObject
 * @param proto
 * @return
 */
std::unique_ptr<ProceduralObjectInterface> ToProceduralObject(ProceduralObjectProto const &proto);

} // namespace e8

#endif // ISLANDS_CONTENT_PROCEDURAL_OBJECT_H
