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

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "content/common.h"
#include "renderer/basic/command_buffer.h"
#include "renderer/dag/graphics_pipeline.h"
#include "renderer/dag/graphics_pipeline_output.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/space_screen/screen_space_processor.h"
#include "renderer/transfer/context.h"
#include "renderer/transfer/uniform_promise.h"
#include "renderer/transfer/vram_uniform.h"

namespace e8 {
namespace {

unsigned const kFramePackageSlot = 0;
unsigned const kRenderPassPackageSlot = 1;

ShaderUniformPackageBindings ViewportDimensionBinding() {
    VkDescriptorSetLayoutBinding viewport_binding;
    viewport_binding.binding = 0;
    viewport_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    viewport_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    viewport_binding.descriptorCount = 1;

    return ShaderUniformPackageBindings{viewport_binding};
}

std::unique_ptr<ShaderUniformLayout>
UniformLayout(unsigned input_image_count, unsigned push_constant_size, VulkanContext *context) {
    ShaderUniformPackageBindings input_image_bindings;
    input_image_bindings.reserve(input_image_count);

    for (unsigned i = 0; i < input_image_count; ++i) {
        VkDescriptorSetLayoutBinding input_image_binding{};
        input_image_binding.binding = i;
        input_image_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        input_image_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        input_image_binding.descriptorCount = 1;

        input_image_bindings.push_back(input_image_binding);
    }

    std::optional<VkPushConstantRange> push_constant_range;
    if (push_constant_size > 0) {
        VkPushConstantRange range{};
        range.offset = 0;
        range.size = push_constant_size;
        range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        push_constant_range.emplace(range);
    }

    return CreateShaderUniformLayout(
        push_constant_range,
        std::vector<ShaderUniformPackageBindings>{ViewportDimensionBinding(), input_image_bindings},
        context);
}

class FrameUniforms : public FrameUniformsInterface {
  public:
    FrameUniforms(unsigned viewport_width, unsigned viewport_height, VulkanContext *vulkan_context)
        : FrameUniformsInterface(kFramePackageSlot) {
        ViewportDimension dimension;
        dimension.viewport_width = viewport_width;
        dimension.viewport_height = viewport_height;

        StagingUniformBuffer viewport_dimension_uniform(
            /*binding=*/0, /*size=*/sizeof(ViewportDimension), vulkan_context);
        viewport_dimension_uniform.WriteObject(dimension);
        package_.buffers.push_back(viewport_dimension_uniform);
    }

    UniformPackage Uniforms() const override { return package_; }

  private:
    struct ViewportDimension {
        float viewport_width;
        float viewport_height;
    };

    UniformPackage package_;
};

class RenderPassUniforms : public RenderPassUniformsInterface {
  public:
    RenderPassUniforms(PipelineKey const &pipeline_key, unsigned input_image_count,
                       VulkanContext *vulkan_context)
        : RenderPassUniformsInterface(GenerateUuidFor(pipeline_key), kRenderPassPackageSlot),
          input_image_sampler_(CreateContinuousSampler(vulkan_context)),
          pending_uniform_data_(nullptr) {}

    void PendUniformData(ScreenSpaceUniformsInterface const *uniform_data) {
        pending_uniform_data_ = uniform_data;
    }

    UniformPackage Uniforms() const override {
        UniformPackage result;
        if (pending_uniform_data_ == nullptr) {
            return result;
        }
    }

    std::vector<uint8_t> UniformPushConstants() const override {
        std::vector<uint8_t> result;
        if (pending_uniform_data_ == nullptr) {
            return result;
        }

        pending_uniform_data_->PushConstants(&result);
        return result;
    }

  private:
    std::unique_ptr<ImageSampler> input_image_sampler_;
    ScreenSpaceUniformsInterface const *pending_uniform_data_;
};

} // namespace

ScreenSpaceUniformsInterface::ScreenSpaceUniformsInterface() = default;
ScreenSpaceUniformsInterface::~ScreenSpaceUniformsInterface() = default;
void ScreenSpaceUniformsInterface::InputImages(std::vector<VkImageView> * /*input_images*/) const {}
void ScreenSpaceUniformsInterface::PushConstants(std::vector<uint8_t> * /*push_constants*/) const {}

struct ScreenSpaceProcessorPipeline::PostProcessorPipelineImpl {
    PostProcessorPipelineImpl(PipelineKey pipeline_key, unsigned input_image_count,
                              GraphicsPipelineOutputInterface *output,
                              VulkanContext *vulkan_context)
        : pipeline_key(pipeline_key),
          frame_uniforms(output->Width(), output->Height(), vulkan_context),
          render_pass_uniforms(pipeline_key, input_image_count, vulkan_context) {}

    ~PostProcessorPipelineImpl() = default;

    PipelineKey pipeline_key;
    FrameUniforms frame_uniforms;
    RenderPassUniforms render_pass_uniforms;
};

ScreenSpaceProcessorPipeline::ScreenSpaceProcessorPipeline(PipelineKey const &pipeline_key,
                                                           std::string const &fragment_shader,
                                                           unsigned input_image_count,
                                                           unsigned push_constant_size,
                                                           GraphicsPipelineOutputInterface *output,
                                                           VulkanContext *vulkan_context)
    : GraphicsPipelineInterface(vulkan_context) {
    shader_stages_ = CreateShaderStages(
        /*vertex_shader_file_path=*/kVertexShaderFilePathPostProcessor,
        /*fragment_shader_file_path=*/fragment_shader, vulkan_context);
    uniform_layout_ = UniformLayout(input_image_count, push_constant_size, vulkan_context);
    vertex_inputs_ = CreateVertexInputState(
        /*input_attributes=*/std::vector<VkVertexInputAttributeDescription>());
    fixed_stage_config_ = CreateFixedStageConfig(/*polygon_mode=*/VK_POLYGON_MODE_FILL,
                                                 /*cull_mode=*/VK_CULL_MODE_NONE,
                                                 /*enable_depth_test=*/false, output->Width(),
                                                 output->Height(), /*color_attachment_count=*/1);

    // Creates the screen space processing pipeline.
    pipeline_ = CreateGraphicsPipeline(output->GetRenderPass(), *shader_stages_, *uniform_layout_,
                                       *vertex_inputs_, *fixed_stage_config_, vulkan_context);

    // Sets up additional states for defining the uniform variables.
    pimpl_ = std::make_unique<PostProcessorPipelineImpl>(pipeline_key, input_image_count, output,
                                                         vulkan_context);
}

ScreenSpaceProcessorPipeline::~ScreenSpaceProcessorPipeline() = default;

PipelineKey ScreenSpaceProcessorPipeline::Key() const { return pimpl_->pipeline_key; }

void ScreenSpaceProcessorPipeline::Launch(GraphicsPipelineArgumentsInterface const &generic_args,
                                          GraphicsPipelineOutputInterface *output,
                                          TransferContext *transfer_context,
                                          CommandBuffer *command_buffer) {
    auto uniform_data = static_cast<ScreenSpaceUniformsInterface const &>(generic_args);
    pimpl_->render_pass_uniforms.PendUniformData(&uniform_data);

    StartRenderPass(output->GetRenderPass(), *output->GetFrameBuffer(), command_buffer);
    PostProcess(*pipeline_, *uniform_layout_, pimpl_->frame_uniforms, pimpl_->render_pass_uniforms,
                transfer_context, command_buffer);
    FinishRenderPass(command_buffer);
}

} // namespace e8
