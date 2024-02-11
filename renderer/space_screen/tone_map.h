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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_TONE_MAP_H
#define ISLANDS_RENDERER_SPACE_SCREEN_TONE_MAP_H

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief DoToneMapping Maps radiance values to the displayable color range. Besides that, it also
 * attaches the luminance value to the alpha channel for potential screen space processing edge AA
 * application.
 *
 * @param radiance_map The radiance values to be mapped.
 * @param exposure Optional. It only uses the ACES tone mapper when the exposure value is provided.
 * Otherwise, it simply clamps the radiance value into the [0, 1] range.
 * @param transfer_context Transfer context.
 * @param dag DAG context.
 * @return The target which tone maps the radiance map to an LDR color image.
 */
DagOperationInstance DoToneMapping(DagOperationInstance radiance_map, DagOperationInstance exposure,
                                   TransferContext *transfer_context, DagContext *dag);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_TONE_MAP_H
