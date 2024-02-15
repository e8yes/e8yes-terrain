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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_EXPOSURE_H
#define ISLANDS_RENDERER_SPACE_SCREEN_EXPOSURE_H

#include <memory>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"

namespace e8 {

/**
 * @brief DoEstimateExposure Estimates the exposure of a radiance map. Precisely, it transforms the
 * specified radiance map into a logarithmic luminance map, then it computes the average of the
 * logarithmic luminance.
 *
 * @param radiance_map The raw radiance image to estimate exposure for.
 * @param transfer_context Transfer context.
 * @param dag DAG context.
 * @return The target stage which stares the logarithmic exposure value.
 */
DagOperationInstance DoEstimateExposure(DagOperationInstance radiance_map, DagContext *dag);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_EXPOSURE_H
