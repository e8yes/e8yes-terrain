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

#ifndef ISLANDS_RENDERER_H
#define ISLANDS_RENDERER_H

#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "content/proto/renderer.pb.h"
#include "content/scene.h"
#include "renderer/context.h"
#include "renderer/pipeline_output.h"

namespace e8 {

/**
 * @brief The RendererInterface class
 */
class RendererInterface {
  public:
    /**
     * @brief The StagePerformance struct Performanec statistics of a client defined stage.
     */
    struct StagePerformance {
        StagePerformance();
        ~StagePerformance();

        // The name of this stage.
        std::string name;

        // The amount of time this stage spent in the last frame, measured in milliseconds.
        float last_1_frame_ms;

        // The average amount of time this stage spent in the last 10 frames, measured in
        // milliseconds.
        float last_10_frame_ms;

        // The average amount of time this stage spent in the last 100 frames, measured in
        // milliseconds.
        float last_100_frame_ms;

        // The average amount of time this stage spent in the last 1000 frames, measured in
        // milliseconds.
        float last_1000_frame_ms;

        // When did this stage begin?
        std::chrono::high_resolution_clock::time_point begin_at;
    };

    /**
     * @brief RendererInterface Constructs a base class.
     *
     * @param num_stages The total number of stages of interest for a renderer subclass.
     * @param max_frame_duration The greatest possible amount of time the frame's rendering process
     * will take before failing this function. This avoids the function from hanging without bound.
     * @param context Contextual Vulkan handles.
     */
    RendererInterface(unsigned num_stages, std::chrono::nanoseconds const &max_frame_duration,
                      VulkanContext *context);
    virtual ~RendererInterface();

    /**
     * @brief DrawFrame Generates a representation of the scene content on the Vulkan swapchain
     * image.
     */
    virtual void DrawFrame(Scene *scene) = 0;

    /**
     * @brief GetPerformanceStats Returns the performance stats for each stage of interest.
     */
    std::vector<StagePerformance> GetPerformanceStats() const;

  protected:
    /**
     * @brief The FrameContext struct Stores information about the swap chain image on which a new
     * frame of rendering process will draw.
     */
    struct FrameContext {
        /**
         * @brief FrameContext Should be created only by calling BeginFrame().
         */
        FrameContext(VulkanContext *context);
        FrameContext(FrameContext const &) = delete;
        FrameContext(FrameContext &&) = default;

        // When the swap chain image indexed by the swap_chain_image_index becomes available, the
        // barrier will be signaled.
        GpuBarrier acquire_swap_chain_image_barrier;

        // The index of the swap chain image which will soon be available for presentation.
        unsigned swap_chain_image_index;
    };

    /**
     * @brief BeginFrame Makes preparation for a new frame.
     */
    FrameContext BeginFrame();

    /**
     * @brief EndFrame Marks the completion of a frame's rendering process.
     *
     * @param frame_context
     * @param final_ouput The last output to be fulfilled before the frame can be presented.
     */
    void EndFrame(FrameContext const &frame_context, PipelineOutputInterface *final_ouput);

    /**
     * @brief BeginStage Marks the beginning of a stage.
     */
    void BeginStage(unsigned index, std::string const &name);

    /**
     * @brief EndStage Marks the end of a stage and record the performanec stats measured.
     */
    void EndStage(unsigned index);

  protected:
    // Contextual Vulkan handles.
    VulkanContext *context;

  private:
    std::chrono::nanoseconds max_frame_duration;
    std::vector<StagePerformance> stage_performance_;
    std::unique_ptr<std::mutex> mu_;
};

/**
 * @brief CreateRenderer Creates a renderer of the specified type.
 */
std::unique_ptr<RendererInterface> CreateRenderer(RendererType type, VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_H
