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

#ifndef ISLANDS_EDITOR_COMPONENT_SCENE_H
#define ISLANDS_EDITOR_COMPONENT_SCENE_H

#include <memory>

#include "content/scene.h"

namespace e8 {

/**
 * @brief The EditorContext struct Stores the editor's global states.
 */
struct EditorContext {
    EditorContext();
    ~EditorContext();

    // The scene the editor is currently working on, it may be null.
    std::unique_ptr<SceneInterface> scene;

    // Indicates if the editor is running.
    bool running;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_SCENE_H
