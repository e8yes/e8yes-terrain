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

#ifndef ISLANDS_RENDERER_SPACE_SCREEN_GAUSSIAN_BLUR_H
#define ISLANDS_RENDERER_SPACE_SCREEN_GAUSSIAN_BLUR_H

#include <memory>

#include "renderer/dag/dag_context.h"
#include "renderer/dag/dag_operation.h"

namespace e8 {

/**
 * @brief The GaussianBlurLevel enum Available gaussian blurring kernel sizes.
 */
enum GaussianBlurLevel {
    GBL_1X1,
    GBL_3X3,
    GBL_5X5,
};

/**
 * @brief DoGaussianBlur Schedules a screen space processing graphics pipeline to Gaussian blur a
 * float map.
 *
 * @param image The floating point image to be blurred.
 * @param blur_level The size of the Gaussian blurring kernel.
 * @param session The DAG session.
 * @return The operation which performs Gaussian blurring off of the input image.
 */
DagOperationInstance DoGaussianBlur(DagOperationInstance image, GaussianBlurLevel blur_level,
                                    DagContext::Session *session);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_SCREEN_GAUSSIAN_BLUR_H
