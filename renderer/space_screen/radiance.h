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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_RADIANCE_H
#define ISLANDS_RENDERER_SPACE_SCREEN_RADIANCE_H

#include <memory>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/drawable/light_source.h"

namespace e8 {

/**
 * @brief DoComputeRadiance Computes the radiance produced by the specified light source based on
 * screen-space geometric and material parameters. Note, the light source must be defined in the
 * view space where the light inputs were generated.
 *
 * @param instance The light source to compute direct radiance for.
 * @param projected_surface Lighting parameters mapped to the screen.
 * @param projection The frustum in which the light inputs are rendered.
 * @param shadow_maps Optional. An array of exponentiated depth maps rendered from the light
 * source's perspective. Each depth map corresponds to one of the light source's region
 * (see LightVolume). When it is specified, this function computes the radiance based on the
 * occlusion factor. Otherwise, the radiance penetrates all objects.
 * @param session The DAG session.
 * @return The operation which computes the radiance map in an HDR color image.
 */
DagOperationInstance DoComputeRadiance(LightSourceInstance const &instance,
                                       DagOperationInstance projected_surface,
                                       frustum const &projection,
                                       std::vector<DagOperationInstance> const &shadow_maps,
                                       DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_RADIANCE_H
