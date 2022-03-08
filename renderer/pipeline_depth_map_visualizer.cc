
#include <cassert>
#include <memory>
#include <optional>
#include <vulkan/vulkan.h>

#include "renderer/context.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_depth_map_visualizer.h"
#include "renderer/pipeline_output.h"
#include "renderer/pipeline_post_processor.h"

namespace e8 {
namespace {

struct SamplerResult {
    SamplerResult(VulkanContext *context);
    ~SamplerResult();

    VkSampler sampler;

    VulkanContext *context;
};

SamplerResult::SamplerResult(VulkanContext *context) : context(context) {}

SamplerResult::~SamplerResult() {
    vkDestroySampler(context->device, sampler, /*pAllocator=*/nullptr);
}

std::unique_ptr<SamplerResult> CreateDepthMapSampler(VulkanContext *context) {
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.magFilter = VK_FILTER_NEAREST;
    sampler_info.minFilter = VK_FILTER_NEAREST;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    auto info = std::make_unique<SamplerResult>(context);
    assert(VK_SUCCESS ==
           vkCreateSampler(context->device, &sampler_info, /*pAllocator=*/nullptr, &info->sampler));

    return info;
}

std::unique_ptr<ShaderUniformLayout> UniformLayout(VulkanContext *context) {
    VkDescriptorSetLayoutBinding depth_map_binding{};
    depth_map_binding.binding = 0;
    depth_map_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    depth_map_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    depth_map_binding.descriptorCount = 1;

    return CreateShaderUniformLayout(
        /*push_constant=*/std::nullopt,
        /*per_frame_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(),
        /*per_pass_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>{depth_map_binding},
        /*per_drawable_desc_set=*/std::vector<VkDescriptorSetLayoutBinding>(), context);
}

struct DescriptorSetResult {
    DescriptorSetResult(VulkanContext *context);
    ~DescriptorSetResult();

    VkDescriptorSet depth_map_set;

    VulkanContext *context;
};

DescriptorSetResult::DescriptorSetResult(VulkanContext *context) : context(context) {}

DescriptorSetResult::~DescriptorSetResult() {
    vkFreeDescriptorSets(context->device, context->descriptor_pool,
                         /*descriptorSetCount=*/1, &depth_map_set);
}

std::unique_ptr<DescriptorSetResult> CreateDescriptorSet(ShaderUniformLayout const &uniform_layout,
                                                         VulkanContext *context) {
    auto info = std::make_unique<DescriptorSetResult>(context);

    VkDescriptorSetAllocateInfo desc_set_alloc_info{};
    desc_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    desc_set_alloc_info.descriptorPool = context->descriptor_pool;
    desc_set_alloc_info.pSetLayouts = &uniform_layout.per_pass_desc_set;
    desc_set_alloc_info.descriptorSetCount = 1;

    assert(VK_SUCCESS ==
           vkAllocateDescriptorSets(context->device, &desc_set_alloc_info, &info->depth_map_set));

    return info;
}

} // namespace

struct DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl {
    DepthMapVisualizerPipelineImpl(PipelineOutputInterface *visualizer_output,
                                   VulkanContext *context);
    ~DepthMapVisualizerPipelineImpl();

    VulkanContext *context;

    std::unique_ptr<SamplerResult> depth_map_sampler;
    std::unique_ptr<DescriptorSetResult> desc_set;

    std::unique_ptr<PostProcessorPipeline> post_processor_pipeline;
};

DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl::DepthMapVisualizerPipelineImpl(
    PipelineOutputInterface *visualizer_output, VulkanContext *context)
    : context(context) {
    depth_map_sampler = CreateDepthMapSampler(context);

    std::unique_ptr<ShaderUniformLayout> uniform_layout = UniformLayout(context);
    desc_set = CreateDescriptorSet(*uniform_layout, context);

    post_processor_pipeline = std::make_unique<PostProcessorPipeline>(
        kFragmentShaderFilePathDepthMapRaw, std::move(uniform_layout), visualizer_output, context);
}

DepthMapVisualizerPipeline::DepthMapVisualizerPipelineImpl::~DepthMapVisualizerPipelineImpl() {}

DepthMapVisualizerPipeline::DepthMapVisualizerPipeline(PipelineOutputInterface *visualizer_output,
                                                       VulkanContext *context)
    : pimpl_(std::make_unique<DepthMapVisualizerPipelineImpl>(visualizer_output, context)) {}

DepthMapVisualizerPipeline::~DepthMapVisualizerPipeline() {}

PipelineOutputInterface *DepthMapVisualizerPipeline::Run(PipelineOutputInterface const &depth_map) {
    return pimpl_->post_processor_pipeline->Run(
        *depth_map.barrier, /*set_uniforms_fn=*/
        [this, &depth_map](ShaderUniformLayout const &uniform_layout, VkCommandBuffer cmds) {
            VkDescriptorImageInfo depth_map_image_info{};
            depth_map_image_info.sampler = this->pimpl_->depth_map_sampler->sampler;
            depth_map_image_info.imageView = depth_map.DepthAttachment()->view;
            depth_map_image_info.imageLayout = depth_map.DepthAttachment()->desc.finalLayout;

            VkWriteDescriptorSet write_depth_map_descriptor{};
            write_depth_map_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write_depth_map_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            write_depth_map_descriptor.dstSet = this->pimpl_->desc_set->depth_map_set;
            write_depth_map_descriptor.dstBinding = 0;
            write_depth_map_descriptor.descriptorCount = 1;
            write_depth_map_descriptor.pImageInfo = &depth_map_image_info;

            vkUpdateDescriptorSets(this->pimpl_->context->device, /*descriptorWriteCount=*/1,
                                   &write_depth_map_descriptor,
                                   /*descriptorCopyCount=*/0, /*pDescriptorCopies=*/nullptr);

            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                    /*firstSet=*/1,
                                    /*descriptorSetCount=*/1,
                                    &this->pimpl_->desc_set->depth_map_set,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        });
}

} // namespace e8
