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

#ifndef ISLANDS_RENDERER_POSTPROCESSOR_TONE_MAP_H
#define ISLANDS_RENDERER_POSTPROCESSOR_TONE_MAP_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief CreateLdrImageStage Creates an LDR (low dynamic range) image stage with a 32-bit RGBA
 * color image output in the specified dimension.
 *
 * @param width The width of the color image.
 * @param height The height of the color image.
 * @param context Contextual Vulkan handles.
 * @return An LDR image stage with a color image output.
 */
std::unique_ptr<PipelineStage> CreateLdrImageStage(unsigned width, unsigned height,
                                                   VulkanContext *context);

/**
 * @brief DoToneMapping Maps radiance values to the displayable color range. Besides that, it also
 * attaches the luminance value to the alpha channel for potential post processing edge AA
 * application.
 *
 * @param radiance_map The radiance values to be mapped.
 * @param exposure Optional. It only uses the ACES tone mapper when the exposure value is provided.
 * Otherwise, it simply clamps the radiance value into the [0, 1] range.
 * @param transfer_context Transfer context.
 * @param target The target stage which stores the tone mapped LDR color image. It should be created
 * through CreateLdrImageStage().
 */
void DoToneMapping(PipelineStage *radiance_map, PipelineStage *exposure,
                   TransferContext *transfer_context, PipelineStage *target);

}  // namespace e8

#endif  // ISLANDS_RENDERER_POSTPROCESSOR_TONE_MAP_H
