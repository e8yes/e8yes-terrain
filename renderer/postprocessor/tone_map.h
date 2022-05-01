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

#ifndef ISLANDS_RENDERER_TONE_MAP_H
#define ISLANDS_RENDERER_TONE_MAP_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/postprocessor/exposure.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The ToneMapPipeline class Maps radiance values to displayable color range.
 */
class ToneMapPipeline {
  public:
    ToneMapPipeline(DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~ToneMapPipeline();

    /**
     * @brief Run Runs the tone mapping pipeline.
     *
     * @param radiance The radiance values to be mapped.
     * @param exposure The pipeline only uses the ACES tone mapper when the exposure value is
     * provided. Otherwise, it simply clamps the radiance value into the [0, 1] range.
     * @param output To store the tone mapped colors.
     */
    void Run(HdrColorPipelineOutput const &radiance,
             ExposureEstimationPipelineOutput const *exposure, PipelineOutputInterface *output);

  private:
    struct ToneMapPipelineImpl;

    DescriptorSetAllocator *desc_set_allocator_;
    VulkanContext *context_;
    PipelineOutputInterface *current_output_;
    std::unique_ptr<ToneMapPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_TONE_MAP_H
