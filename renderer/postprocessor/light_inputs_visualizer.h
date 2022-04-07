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

#ifndef ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_VISUALIZER_H
#define ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_VISUALIZER_H

#include <memory>

#include "common/device.h"
#include "renderer/pipeline/light_inputs.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/transfer/descriptor_set.h"

namespace e8 {

/**
 * @brief The LightInputsVisualizerPipeline class A post processing graphics pipeline for
 * visualizing a rendered depth map.
 */
class LightInputsVisualizerPipeline {
  public:
    /**
     * @brief LightInputsVisualizerPipeline Constructs a post processing graphics pipeline for
     * visualizing a rendered light inputs map.
     *
     * @param visualizer_output An object for storing the depth map image in grayscale.
     * @param context Contextual Vulkan handles.
     */
    LightInputsVisualizerPipeline(PipelineOutputInterface *visualizer_output,
                                  DescriptorSetAllocator *desc_set_allocator,
                                  VulkanContext *context);

    ~LightInputsVisualizerPipeline();

    /**
     * @brief Run Runs the light inputs visualizer graphics pipeline. The pipeline can only be run
     * when the previous run was finished (indicated by the output's barrier).
     *
     * @param input_to_visualize Selects the input to be visualized.
     * @param light_inputs The light inputs map to be visualized.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(LightInputsRendererParameters::InputType input_to_visualize,
                                 LightInputsPipelineOutput const &light_inputs);

  private:
    struct LightInputsVisualizerPipelineImpl;
    std::unique_ptr<LightInputsVisualizerPipelineImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_LIGHT_INPUTS_VISUALIZER_H
