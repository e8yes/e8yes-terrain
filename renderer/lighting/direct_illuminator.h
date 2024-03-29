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

#ifndef ISLANDS_RENDERER_LIGHTING_DIRECT_ILLUMINATOR_H
#define ISLANDS_RENDERER_LIGHTING_DIRECT_ILLUMINATOR_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "renderer/basic/projection.h"
#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/drawable/collection.h"

namespace e8 {

/**
 * @brief DoComputeDirectIllumination Computes the direct illumination from a list of light
 * sources on a map of lighting parameters.
 *
 * @param drawable_collection Light sources and geometries for which direct illumination needs
 * to be resolved.
 * @param projected_surface Lighting parameters projected to the screen.
 * @param projection The perspective projection setup which generated the surface parameter
 * map.
 * @param session The DAG session.
 * @return The direct illumination result. Note, the radiance map is cleared to black prior to
 * the direct illumination computation.
 */
DagOperationInstance DoComputeDirectIllumination(DrawableCollection *drawable_collection,
                                                 DagOperationInstance projected_surface,
                                                 PerspectiveProjection const &projection,
                                                 DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_LIGHTING_DIRECT_ILLUMINATOR_H
