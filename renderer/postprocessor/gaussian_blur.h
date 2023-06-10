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

#ifndef ISLANDS_RENDERER_POSTPROCESSOR_SHADOW_MAP_H
#define ISLANDS_RENDERER_POSTPROCESSOR_SHADOW_MAP_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/transfer/context.h"

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
 * @brief CreateGaussianBlurStages Creates a pair of separated Gaussian blurring post-processor
 * stages with 32-bit floating point outputs in the specified dimension.
 *
 * @param width The width of the Gaussian blurred float map output.
 * @param height The height of the Gaussian blurred float map output.
 * @param context Contextual Vulkan handles.
 * @param h_blurred The horizontally blurred temporary output.
 * @param hv_blurred The final output with both horizontal and vertical blurring.
 */
void CreateGaussianBlurStages(unsigned width, unsigned height, VulkanContext *context,
                              std::unique_ptr<PipelineStage> *h_blurred,
                              std::unique_ptr<PipelineStage> *hv_blurred);

/**
 * @brief DoGaussianBlur Schedules a post processing graphics pipeline to Gaussian blur a float map.
 *
 * @param image The floating point image to be blurred.
 * @param blur_level The size of the Gaussian blurring kernel.
 * @param transfer_context Transfer context.
 * @param h_blurred The horizontally blurred temporary output.
 * @param hv_blurred The final output with both horizontal and vertical blurring.
 */
void DoGaussianBlur(PipelineStage *image, GaussianBlurLevel blur_level,
                    TransferContext *transfer_context, PipelineStage *h_blurred,
                    PipelineStage *hv_blurred);

} // namespace e8

#endif // ISLANDS_RENDERER_POSTPROCESSOR_SHADOW_MAP_H
