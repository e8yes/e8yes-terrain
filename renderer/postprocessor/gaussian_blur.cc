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

#include <memory>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/postprocessor/gaussian_blur.h"
#include "renderer/transfer/context.h"

namespace e8 {

void CreateGaussianBlurStages(unsigned width, unsigned height, VulkanContext* context,
                              std::unique_ptr<PipelineStage>* h_blurred,
                              std::unique_ptr<PipelineStage>* hv_blurred) {}

void DoGaussianBlur(PipelineStage* image, unsigned blur_kernel_size,
                    TransferContext* transfer_context, PipelineStage* h_blurred,
                    PipelineStage* hv_blurred) {}

}  // namespace e8
