#ifndef PIPELINE_DEPTH_MAP_VISUALIZER_H
#define PIPELINE_DEPTH_MAP_VISUALIZER_H

#include <memory>

#include "renderer/context.h"
#include "renderer/pipeline_output.h"

namespace e8 {

/**
 * @brief The DepthMapVisualizerPipeline class A post processing graphics pipeline for
 * visualizing a rendered depth map.
 */
class DepthMapVisualizerPipeline {
  public:
    /**
     * @brief DepthMapVisualizerPipeline Constructs a post processing graphics pipeline for
     * visualizing a rendered depth map.
     *
     * @param visualizer_output An object for storing the depth map image in grayscale.
     * @param context Contextual Vulkan handles.
     */
    DepthMapVisualizerPipeline(PipelineOutputInterface *visualizer_output, VulkanContext *context);

    ~DepthMapVisualizerPipeline();

    /**
     * @brief Run Runs the depth map visualizer graphics pipeline. The pipeline can only be run when
     * the previous run was finished (indicated by the output's barrier).
     *
     * @param depth_map The depth map to be visualized.
     * @return The output object set from the constructor, with a barrier assigned.
     */
    PipelineOutputInterface *Run(PipelineOutputInterface const &depth_map);

  private:
    struct DepthMapVisualizerPipelineImpl;
    std::unique_ptr<DepthMapVisualizerPipelineImpl> pimpl_;
};

} // namespace e8

#endif // PIPELINE_DEPTH_MAP_VISUALIZER_H
