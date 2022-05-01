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

#ifndef ISLANDS_RENDERER_FXAA_H
#define ISLANDS_RENDERER_FXAA_H

#include <memory>

#include "common/device.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The FxaaPipeline class Reduces edge aliasing artifact through a post processing technique
 * devised by Timothy Lottes.
 */
class FxaaPipeline {
  public:
    FxaaPipeline(DescriptorSetAllocator *desc_set_allocator, VulkanContext *context);
    ~FxaaPipeline();

    /**
     * @brief Run Runs the FXAA pipeline.
     *
     * @param input An LDR RGB image where the alpha channel is populated with the color's
     * luminance value.
     * @param output An edge anti-aliased LDR image with which the alpha channel is cleared
     * to 1.0.
     */
    void Run(PipelineOutputInterface const &input, PipelineOutputInterface *output);

  private:
    struct FxaaPipelineImpl;

    DescriptorSetAllocator *desc_set_allocator_;
    VulkanContext *context_;
    PipelineOutputInterface *current_output_;
    std::unique_ptr<FxaaPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_FXAA_H
