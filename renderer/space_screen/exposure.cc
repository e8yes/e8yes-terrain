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

#include <cassert>
#include <memory>
#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/attachment.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/mipmap.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/shader.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/dag/promise.h"
#include "renderer/space_screen/exposure.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/transfer/context.h"

namespace e8 {
namespace {

PipelineKey const kLogarithmicLuminancePipeline = "Logarithmic Luminance";
PipelineKey const kComputeAveragePipeline = "Compute Average";

using GpuAndCpuPromise = std::pair<std::unique_ptr<GpuPromise>, std::unique_ptr<CpuPromise>>;

/**
 * @brief The LogLuminanceOutput class Stores the logarithmic luminance value of each radiance
 * pixel.
 */
class LogLuminancePipelineOutput : public GraphicsPipelineOutputInterface {
  public:
    LogLuminancePipelineOutput(unsigned width, unsigned height, VulkanContext *context);
    ~LogLuminancePipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    std::unique_ptr<FrameBufferAttachment> color_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
    std::unique_ptr<FrameBuffer> frame_buffer_;
};

LogLuminancePipelineOutput::LogLuminancePipelineOutput(unsigned width, unsigned height,
                                                       VulkanContext *context)
    : GraphicsPipelineOutputInterface(width, height) {
    color_attachment_ = CreateColorAttachment(width, height, VkFormat::VK_FORMAT_R16_SFLOAT,
                                              /*transfer_src=*/true, context);
    render_pass_ = CreateRenderPass(std::vector<VkAttachmentDescription>{color_attachment_->desc},
                                    /*depth_attachment_desc=*/std::nullopt, context);
    frame_buffer_ = CreateFrameBuffer(*render_pass_, width, height,
                                      std::vector<VkImageView>{color_attachment_->view},
                                      /*depth_attachment_desc=*/std::nullopt, context);
}

LogLuminancePipelineOutput::~LogLuminancePipelineOutput() {}

FrameBuffer *LogLuminancePipelineOutput::GetFrameBuffer() const { return frame_buffer_.get(); }

RenderPass const &LogLuminancePipelineOutput::GetRenderPass() const { return *render_pass_; }

std::vector<FrameBufferAttachment const *> LogLuminancePipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{color_attachment_.get()};
}

FrameBufferAttachment const *LogLuminancePipelineOutput::DepthAttachment() const { return nullptr; }

/**
 * @brief The ExposureEstimationPipelineOutput class Stores the logarithmic lumiance average in a
 * 1x1 image as the exposure of the radiance map.
 */
class ExposureEstimationPipelineOutput : public GraphicsPipelineOutputInterface {
  public:
    explicit ExposureEstimationPipelineOutput(VulkanContext *context);
    ~ExposureEstimationPipelineOutput();

    FrameBuffer *GetFrameBuffer() const override;
    RenderPass const &GetRenderPass() const override;
    std::vector<FrameBufferAttachment const *> ColorAttachments() const override;
    FrameBufferAttachment const *DepthAttachment() const override;

  private:
    std::unique_ptr<FrameBufferAttachment> color_attachment_;
    std::unique_ptr<RenderPass> render_pass_;
};

ExposureEstimationPipelineOutput::ExposureEstimationPipelineOutput(VulkanContext *context)
    : GraphicsPipelineOutputInterface(/*width=*/1, /*height=*/1) {
    color_attachment_ = CreateStorageAttachment(/*width=*/1, /*height=*/1,
                                                /*format=*/VK_FORMAT_R16_SFLOAT, context);
    render_pass_ = std::make_unique<RenderPass>(context);
}

ExposureEstimationPipelineOutput::~ExposureEstimationPipelineOutput() {}

FrameBuffer *ExposureEstimationPipelineOutput::GetFrameBuffer() const { return nullptr; }

RenderPass const &ExposureEstimationPipelineOutput::GetRenderPass() const { return *render_pass_; }

std::vector<FrameBufferAttachment const *>
ExposureEstimationPipelineOutput::ColorAttachments() const {
    return std::vector<FrameBufferAttachment const *>{color_attachment_.get()};
}

FrameBufferAttachment const *ExposureEstimationPipelineOutput::DepthAttachment() const {
    return nullptr;
}

/**
 * @brief The LogLuminanceConfigurator class For setting up the log luminance shader's uniform
 * variables.
 */
class LogLuminanceConfigurator : public ScreenSpaceConfiguratorInterface {
  public:
    LogLuminanceConfigurator(GraphicsPipelineOutputInterface const &radiance);
    ~LogLuminanceConfigurator() override;

    void InputImages(std::vector<VkImageView> *input_images) const override;

  private:
    GraphicsPipelineOutputInterface const &radiance_;
};

LogLuminanceConfigurator::LogLuminanceConfigurator(GraphicsPipelineOutputInterface const &radiance)
    : radiance_(radiance) {}

LogLuminanceConfigurator::~LogLuminanceConfigurator() {}

void LogLuminanceConfigurator::InputImages(std::vector<VkImageView> *input_images) const {
    input_images->at(0) = radiance_.ColorAttachments()[0]->view;
}

/**
 * @brief The ComputeAveragePipelineArguments struct Arguments to the ComputeAveragePipeline.
 */
struct ComputeAveragePipelineArguments : public GraphicsPipelineArgumentsInterface {
    ComputeAveragePipelineArguments(GraphicsPipelineOutputInterface const &log_luminance_map)
        : log_luminance_map(log_luminance_map) {}

    GraphicsPipelineOutputInterface const &log_luminance_map;
};

/**
 * @brief The ComputeAveragePipeline class For computing the average value of log luminance map.
 */
class ComputeAveragePipeline : public GraphicsPipelineInterface {
  public:
    ComputeAveragePipeline(VulkanContext *context) : GraphicsPipelineInterface(context) {}
    ~ComputeAveragePipeline() override = default;

    PipelineKey Key() const override { return kComputeAveragePipeline; }

    void Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                GraphicsPipelineOutputInterface *exposure_output,
                TransferContext * /*transfer_context*/, CommandBuffer *command_buffer) override {
        ComputeAveragePipelineArguments const &args =
            static_cast<ComputeAveragePipelineArguments const &>(generic_args);

        AverageValueOf(args.log_luminance_map.ColorAttachments()[0]->image,
                       args.log_luminance_map.Width(), args.log_luminance_map.Height(),
                       command_buffer->buffer, exposure_output->ColorAttachments()[0]->image);
    }
};

/**
 * @brief CreateLogLuminanceOp Creates a log luminance operation with a 16-bit float image output in
 * the specified dimension.
 *
 * @param width The width of the log luminance map.
 * @param height The height of the log luminance map.
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the 16-bit float image output.
 */
std::unique_ptr<DagOperation> CreateLogLuminanceOp(unsigned width, unsigned height,
                                                   TransferContext *transfer_context,
                                                   VulkanContext *vulkan_context) {
    auto output = std::make_shared<LogLuminancePipelineOutput>(width, height, vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
}

/**
 * @brief CreateExposureValueOp Creates an exposure value storage via a 1 by 1 16-bit float image
 * output for storing the exposure value.
 *
 * @param context Contextual Vulkan handles.
 * @return A pipeline stage created with the 1 by 1 16-bit float image output.
 */
std::unique_ptr<DagOperation> CreateExposureValueOp(TransferContext *transfer_context,
                                                    VulkanContext *vulkan_context) {
    auto output = std::make_shared<ExposureEstimationPipelineOutput>(vulkan_context);
    return std::make_unique<DagOperation>(output, transfer_context, vulkan_context);
}

} // namespace

DagOperationInstance DoEstimateExposure(DagOperationInstance radiance_map, DagContext *dag) {
    DagContext::DagOperationKey log_luminance_key =
        CreateDagOperationKey(kLogarithmicLuminancePipeline, radiance_map->Output()->Width(),
                              radiance_map->Output()->Height());
    DagContext::DagOperationKey compute_average_key =
        CreateDagOperationKey(kComputeAveragePipeline, /*width=*/1, /*height=*/1);
    DagOperationInstance log_luminance_map =
        dag->WithOperation(log_luminance_key, [radiance_map](TransferContext *transfer_context,
                                                             VulkanContext *vulkan_context) {
            return CreateLogLuminanceOp(radiance_map->Output()->Width(),
                                        radiance_map->Output()->Height(), transfer_context,
                                        vulkan_context);
        });
    DagOperationInstance log_exposure = dag->WithOperation(
        compute_average_key, [](TransferContext *transfer_context, VulkanContext *vulkan_context) {
            return CreateExposureValueOp(transfer_context, vulkan_context);
        });

    GraphicsPipelineInterface *log_luminance_pipeline = log_luminance_map->WithPipeline(
        kLogarithmicLuminancePipeline,
        [](GraphicsPipelineOutputInterface *log_luminance_output, TransferContext *transfer_context,
           VulkanContext *vulkan_context) {
            return std::make_unique<ScreenSpaceProcessorPipeline>(
                kLogarithmicLuminancePipeline, kFragmentShaderFilePathLogLuminance,
                /*input_image_count=*/1, /*push_constant_size=*/0, log_luminance_output,
                transfer_context, vulkan_context);
        });
    GraphicsPipelineInterface *compute_average_pipeline = log_exposure->WithPipeline(
        kComputeAveragePipeline,
        [](GraphicsPipelineOutputInterface * /*exposure_output*/,
           TransferContext * /*transfer_context*/, VulkanContext *vulkan_context) {
            return std::make_unique<ComputeAveragePipeline>(vulkan_context);
        });

    auto configurator = std::make_unique<LogLuminanceConfigurator>(*radiance_map->Output());
    log_luminance_map->Schedule(log_luminance_pipeline, std::move(configurator),
                                /*parents=*/std::vector<DagOperationInstance>{radiance_map});

    auto compute_average_args =
        std::make_unique<ComputeAveragePipelineArguments>(*log_luminance_map->Output());
    log_exposure->Schedule(compute_average_pipeline, std::move(compute_average_args),
                           /*parents=*/std::vector<DagOperationInstance>{log_luminance_map});

    return log_exposure;
}

} // namespace e8
