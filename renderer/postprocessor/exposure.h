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

#ifndef ISLANDS_RENDERER_EXPOSURE_H
#define ISLANDS_RENDERER_EXPOSURE_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief CreateLogLuminaneStage Creates a log luminance stage with a 16-bit float image output in
 * the specified dimension.
 *
 * @param width The width of the log luminance map.
 * @param height The height of the log luminance map.
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the 16-bit float image output.
 */
std::unique_ptr<PipelineStage> CreateLogLuminaneStage(unsigned width, unsigned height,
                                                      VulkanContext *context);

/**
 * @brief CreateExposureStage Creates an exposure stage with a 1 by 1 16-bit float image output for
 * storing the exposure value.
 *
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the 1 by 1 16-bit float image output.
 */
std::unique_ptr<PipelineStage> CreateExposureStage(VulkanContext *context);

/**
 * @brief DoEstimateExposure Estimates the exposure of a radiance map. Precisely, it transforms the
 * specified radiance map into a logarithmic luminance map, then it computes the average of the
 * logarithmic luminance.
 *
 * @param radiance_map The raw radiance image to estimate exposure for.
 * @param transfer_context Transfer context.
 * @param log_luminance_map The target stage which stores the logarithmic luminance values.
 * @param log_exposure The target stage which stares the logarithmic exposure value.
 */
void DoEstimateExposure(PipelineStage *radiance_map, TransferContext *transfer_context,
                        PipelineStage *log_luminance_map, PipelineStage *log_exposure);

}  // namespace e8

#endif  // ISLANDS_RENDERER_EXPOSURE_H
