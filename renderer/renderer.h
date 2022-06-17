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

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "common/device.h"
#include "content/scene.h"
#include "renderer/output/common_output.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/output/promise.h"
#include "renderer/proto/renderer.pb.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief The RendererInterface class Common function prototypes where different renderers share.
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
     * @param type The renderer's type.
     * @param num_stages The total number of stages of interest for a renderer subclass.
     * will take before failing this function. This avoids the function from hanging without bound.
     * @param context Contextual Vulkan handles.
     */
    RendererInterface(unsigned num_stages, VulkanContext *context);
    virtual ~RendererInterface();

    /**
     * @brief DrawFrame Generates a representation of the scene content on the Vulkan swapchain
     * image. Note, most of the scene data is dynamically loaded from disk. Uses the
     * GeometryRamTransfer to access data through a caching layer.
     */
    virtual void DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) = 0;

    /**
     * @brief ApplyConfiguration Applies the specified configuration to the renderer.
     */
    virtual void ApplyConfiguration(RendererConfiguration const &config) = 0;

    /**
     * @brief Type Returns the renderer's type
     */
    RendererType Type() const;

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
        GpuPromise swap_chain_image_promise;

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
     * @param frame_context Created by RendererInterface::BeginFrame().
     * @param final_ouput The last output to be fulfilled before the frame can be presented.
     */
    void EndFrame(FrameContext const &frame_context,
                  std::vector<PipelineOutputInterface *> pipeline_ouputs);

    /**
     * @brief DoFirstStage Creates the first stage of the current frame. The first stage waits for
     * the v-sync.
     */
    PipelineStage *DoFirstStage();

    /**
     * @brief DoFinalStage Creates the final stage of the current frame. The final stage presents
     * the color map to the screen.
     *
     * @param first_stage Returned by RendererInterface::DoFirstStage().
     * @param color_map_stage Returned by RendererInterface::ColorMapStage().
     * @return The final stage.
     */
    PipelineStage *DoFinalStage(PipelineStage * first_stage, PipelineStage* color_map_stage);


    /**
     * @brief ColorMapStage Creates a color map stage. A color map is an image which will be
     * presented as the final rendering result. It's also the last customizable pipeline stage.
     */
    std::unique_ptr<PipelineStage> ColorMapStage() const;

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
    std::vector<StagePerformance> stage_performance_;
    std::shared_ptr<SwapChainPipelineOutput> final_output_;
    PipelineStage first_stage_;
    PipelineStage final_stage_;
    std::unique_ptr<std::mutex> mu_;
};

/**
 * @brief CreateRenderer Creates a renderer of the specified type.
 */
std::unique_ptr<RendererInterface> CreateRenderer(RendererType type, VulkanContext *context);

/**
 * @brief LoadRendererConfiguration Loads renderer configuration from the base_path. If the loading
 * fails, it returns a nullptr.
 */
std::unique_ptr<RendererConfiguration>
LoadRendererConfiguration(std::filesystem::path const &base_path);

/**
 * @brief SaveRendererConfiguration Saves the renderer configuration to disk, under the specified
 * base_path.
 */
bool SaveRendererConfiguration(RendererConfiguration const &config,
                               std::filesystem::path const &base_path);

/**
 * @brief DefaultRendererConfiguration Returns a default configuration.
 */
std::unique_ptr<RendererConfiguration> DefaultRendererConfiguration();

} // namespace e8

#endif // ISLANDS_RENDERER_H
