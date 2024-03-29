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
#include <vector>

#include "common/device.h"
#include "content/scene.h"
#include "renderer/dag/frame_resource_allocator.h"
#include "renderer/dag/graphics_pipeline_output_common.h"
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
     * @brief AcquireFinalColorImage Waits until a swapchain image is available.
     */
    std::shared_ptr<SwapChainOutput>
    AcquireFinalColorImage(FrameResourceAllocator *frame_resource_allocator);

    /**
     * @brief PresentFinalColorImage Presents the specified color image to the screen.
     * @param output The color image to be presented.
     * @param waits A set of signals of operations which must finish prior the presentation.
     */
    void PresentFinalColorImage(SwapChainOutput const &output,
                                std::vector<GpuPromise *> const &final_waits);

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
    std::shared_ptr<SwapChainOutput> final_output_;
    CpuPromise acquisition_signal_;
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
