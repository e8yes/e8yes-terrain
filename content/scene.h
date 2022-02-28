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

#ifndef ISLANDS_RENDERER_SCENE_H
#define ISLANDS_RENDERER_SCENE_H

#include <map>
#include <memory>
#include <shared_mutex>
#include <string>

#include "content/procedural_object.h"
#include "content/proto/scene.pb.h"
#include "content/proto/scene_object.pb.h"
#include "content/scene_object.h"
#include "content/structure.h"

namespace e8 {

// Uniquely identifies a scene.
using SceneId = std::string;

/**
 * @brief The Scene class a container for efficient scene object storage and query. This container
 * isn't thread-safe, but it provides locking device to enable safe concurrent resource access.
 */
class Scene {
  public:
    /**
     * @brief The ReadAccess class Created by calling GainReadAccess(). Note, the access right is
     * valid throughout the life of the ReadAccess object.
     */
    class ReadAccess {
      public:
        ReadAccess(std::shared_mutex *mu);
        ReadAccess(ReadAccess &&other);
        ReadAccess(ReadAccess const &) = delete;
        ~ReadAccess();

      private:
        std::shared_mutex *mu_;
    };

    /**
     * @brief The WriteAccess class Created by calling GainWriteAccess(). Note, the access right is
     * valid throughout the life of the WriteAccess object.
     */
    class WriteAccess {
      public:
        WriteAccess(std::shared_mutex *mu);
        WriteAccess(WriteAccess &&other);
        WriteAccess(WriteAccess const &) = delete;
        ~WriteAccess();

      private:
        std::shared_mutex *mu_;
    };

    /**
     * @brief SceneInterface Constructs an empty scene.
     *
     * @param structure_type The type of structure to use for storing scene entities.
     * @param name A descriptive human readable name for the scene.
     */
    Scene(SceneProto::StructureType structure_type, std::string const &name);

    /**
     * @brief SceneInterface Constructs a scene base class with content provided by the proto
     * object.
     */
    explicit Scene(SceneProto const &proto);

    virtual ~Scene();

    Scene(Scene const &) = delete;

    /**
     * @brief GainReadAccess Gain access to read scene content. Note, the access right is valid
     * throughout the life of the ReadAccess object. Also, the scene must live longer than the
     * returned object.
     */
    ReadAccess GainReadAccess();

    /**
     * @brief GainWriteAccess Gain access to read and write scene content. Note, the access right is
     * valid throughout the life of the WriteAccess object. Also, the scene must live longer than
     * the returned object.
     * @return
     */
    WriteAccess GainWriteAccess();

    /**
     * @brief SceneEntityContainer Gets access to the scene entity structure.
     */
    SceneEntityStructureInterface *SceneEntityStructure();

    /**
     * @brief AddRootSceneObject Adds a new root scene object to the scene if it has not already
     * been added. Otherwise, it will do nothing.
     *
     * @param scene_object The scene object to be added into the scene.
     * @return true only if the scene object has not been added to the scene.
     */
    bool AddRootSceneObject(SceneObject const &scene_object);

    /**
     * @brief DeleteRootSceneObject Deletes a root scene object by ID. It returns true if the scene
     * object exists.
     */
    bool DeleteRootSceneObject(SceneObjectId const &id);

    /**
     * @brief AddProceduralObject
     */
    bool AddProceduralObject(std::unique_ptr<ProceduralObjectInterface> &&procedural_object);

    /**
     * @brief AllRootSceneObjects Returns a list of all root scene objects added to the scene.
     */
    std::map<SceneObjectId, SceneObject> const &AllRootSceneObjects() const;

    /**
     * @brief UpdateBackgroundColor Sets a new background color for the scene.
     */
    void UpdateBackgroundColor(vec3 const &color);

    /**
     * @brief BackgroundColor Returns the scene's current background color.
     */
    vec3 BackgroundColor() const;

    /**
     * @brief ToProto Turns scene content into a protobuf object.
     */
    SceneProto ToProto() const;

  public:
    // Id of the scene.
    SceneId id;

    // A descriptive human readable name of the scene.
    std::string name;

    // The structure used for organizing scene entities efficiently.
    SceneProto::StructureType structure_type;

  private:
    void CreateSceneEntityStructure();

    std::shared_mutex mu_;

    std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>> procedural_objects_;
    std::map<SceneObjectId, SceneObject> root_scene_objects_;
    std::unique_ptr<SceneEntityStructureInterface> entity_structure_;
    vec3 background_color_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_SCENE_H
