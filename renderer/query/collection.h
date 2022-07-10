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

#ifndef ISLANDS_RENDERER_QUERY_COLLECTION_H
#define ISLANDS_RENDERER_QUERY_COLLECTION_H

#include <vector>

#include "content/structure.h"
#include "renderer/basic/projection.h"
#include "renderer/query/drawable_instance.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief The DrawableCollection class A logical collection of all drawables.
 */
class DrawableCollection {
   public:
    DrawableCollection(SceneEntityStructureInterface const& entities,
                       ResourceAccessor* resource_accessor);
    ~DrawableCollection();

    /**
     * @brief ObservableGeometries Returns a list of geometry drawables that can be observed in the
     * given perspective projection.
     */
    std::vector<DrawableInstance> ObservableGeometries(
        PerspectiveProjection const& projection,
        ResourceLoadingOption const& loading_option = ResourceLoadingOption());

   private:
    SceneEntityStructureInterface const& entities_;
    ResourceAccessor* resource_accessor_;
};

}  // namespace e8

#endif  // ISLANDS_RENDERER_QUERY_COLLECTION_H
