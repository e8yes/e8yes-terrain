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

#include "common/tensor.h"
#include "content/drawable.h"

namespace e8 {

/**
 * @brief The SceneInterface class Represents a thread-safe container for efficient scene object
 * storage and query.
 */
class SceneInterface {
  public:
    SceneInterface();
    ~SceneInterface();

    /**
     * @brief AddDrawable Adds a new drawable to the scene.
     *
     * @param drawable The drawable to be added.
     * @return true only if the drawable has not been added to the scene.
     */
    virtual bool AddDrawable(std::shared_ptr<DrawableLod> const &drawable) = 0;

    /**
     * @brief Drawable Finds drawable by ID. If the drawable doesn't exist, it returns a nullptr.
     */
    virtual std::shared_ptr<DrawableLod> FindDrawable(DrawableId const &id) const = 0;

    /**
     * @brief AddSceneObject Adds a new scene object to the scene if it has not already been added.
     * Otherwise, it will do nothing.
     *
     * @param scene_object The scene object to be moved into the scene.
     * @return true only if the scene object has not been added to the scene.
     */
    virtual bool AddSceneObject(SceneObject &&scene_object) = 0;

    /**
     * @brief FindDrawableInstances Filters drawable instances by the frustum area specified. A
     * drawable instance' bounding box must fall within the frustum for it to be selected.
     *
     * @param f Defines the frustum area.
     * @return All the selected instances.
     */
    virtual std::vector<DrawableLodInstance *> FindDrawableInstances(frustum const &f) const = 0;

    /**
     * @brief AllDrawableInstances Returns all the drawable instances that have been added to this
     * scene.
     */
    virtual std::vector<DrawableLodInstance *> AllDrawableInstances() const = 0;
};

} // namespace e8

#endif // ISLANDS_RENDERER_SCENE_H
