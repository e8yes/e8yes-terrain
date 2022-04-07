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
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"
#include "renderer/pass/rasterize.h"
#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/texture_group.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "resource/common.h"
#include "resource/geometry.h"

namespace e8 {
namespace {

std::unordered_set<DrawableInstance const *>
UploadResources(std::vector<DrawableInstance> const &drawables,
                RenderPassConfiguratorInterface const &configurator, GeometryVramTransfer *geo_vram,
                TextureVramTransfer *tex_vram) {
    std::unordered_set<DrawableInstance const *> excluded;

    std::vector<Geometry const *> geometries;
    std::vector<StagingTextureBuffer const *> textures;

    for (auto const &drawable : drawables) {
        if (!configurator.IncludeDrawable(drawable)) {
            excluded.insert(&drawable);
            continue;
        }

        if (!drawable.geometry->Valid()) {
            excluded.insert(&drawable);
            continue;
        }

        TextureSelector texture_selector = configurator.TexturesOf(drawable);
        if (!texture_selector.Valid()) {
            excluded.insert(&drawable);
            continue;
        }

        geometries.push_back(drawable.geometry);
        texture_selector.AppendTo(&textures);
    }

    geo_vram->Upload(geometries);
    tex_vram->Upload(textures);

    return excluded;
}

} // namespace

RenderPassConfiguratorInterface::RenderPassConfiguratorInterface() {}

RenderPassConfiguratorInterface::~RenderPassConfiguratorInterface() {}

bool RenderPassConfiguratorInterface::IncludeDrawable(DrawableInstance const & /*drawable*/) const {
    return true;
}

TextureSelector
RenderPassConfiguratorInterface::TexturesOf(DrawableInstance const & /*drawable*/) const {
    return TextureSelector(kNullUuid);
}

void RenderPassConfiguratorInterface::PushConstant(DrawableInstance const & /*drawable*/,
                                                   VkCommandBuffer /*cmds*/) const {}

VkCommandBuffer StartRenderPass(RenderPass const &render_pass, FrameBuffer const &frame_buffer,
                                VulkanContext *context) {
    // Allocates a new command buffer for storing commands from this render pass.
    VkCommandBufferAllocateInfo cmds_allocation_info{};
    cmds_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmds_allocation_info.commandPool = context->command_pool;
    cmds_allocation_info.commandBufferCount = 1;
    cmds_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmds;
    assert(VK_SUCCESS == vkAllocateCommandBuffers(context->device, &cmds_allocation_info, &cmds));

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    assert(VK_SUCCESS == vkBeginCommandBuffer(cmds, &command_buffer_begin_info));

    // Sets the frame buffer and makes preparation for this render pass.
    VkRenderPassBeginInfo render_pass_begin_info{};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = render_pass.pass;
    render_pass_begin_info.framebuffer = frame_buffer.buffer;
    render_pass_begin_info.renderArea.extent.width = frame_buffer.width;
    render_pass_begin_info.renderArea.extent.height = frame_buffer.height;
    if (!frame_buffer.clear_values.empty()) {
        render_pass_begin_info.pClearValues = frame_buffer.clear_values.data();
        render_pass_begin_info.clearValueCount = frame_buffer.clear_values.size();
    }

    vkCmdBeginRenderPass(cmds, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    return cmds;
}

std::unique_ptr<GpuPromise> FinishRenderPass(VkCommandBuffer cmds, GpuPromise const &prerequisites,
                                             VkFence fence, VulkanContext *context) {
    // Finalizes the render pass and command buffers.
    vkCmdEndRenderPass(cmds);
    assert(VK_SUCCESS == vkEndCommandBuffer(cmds));

    // For render pass synchronization.
    VkSemaphore done_signal;
    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    assert(VK_SUCCESS == vkCreateSemaphore(context->device, &semaphore_info,
                                           /*pAllocator=*/nullptr, &done_signal));

    // Submit command buffers.
    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    if (!prerequisites.tasks_signal.empty()) {
        submit.pWaitSemaphores = prerequisites.tasks_signal.data();
        submit.waitSemaphoreCount = prerequisites.tasks_signal.size();
        submit.pWaitDstStageMask = &wait_stage;
    }
    submit.pSignalSemaphores = &done_signal;
    submit.signalSemaphoreCount = 1;
    submit.pCommandBuffers = &cmds;
    submit.commandBufferCount = 1;

    assert(VK_SUCCESS == vkQueueSubmit(context->graphics_queue, /*submitCount=*/1, &submit, fence));

    return std::make_unique<GpuPromise>(done_signal, cmds, context);
}

void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                     RenderPassConfiguratorInterface const &configurator,
                     TextureDescriptorSetCache *tex_desc_set_cache, GeometryVramTransfer *geo_vram,
                     TextureVramTransfer *tex_vram, VkCommandBuffer cmds) {
    std::unordered_set<DrawableInstance const *> excluded =
        UploadResources(drawables, configurator, geo_vram, tex_vram);

    // Sends drawable instances through the graphics pipeline.
    vkCmdBindPipeline(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
    for (auto const &instance : drawables) {
        if (excluded.find(&instance) != excluded.end()) {
            continue;
        }

        // Fetches GPU geometry data.
        GeometryVramTransfer::GpuGeometry gpu_geometry = geo_vram->Find(instance.geometry);
        if (!gpu_geometry.Valid()) {
            continue;
        }

        // Fetches GPU texture data.
        TextureSelector texture_selector = configurator.TexturesOf(instance);

        TextureGroup texture_group;
        texture_group.id = texture_selector.id;
        texture_group.layout = uniform_layout.per_drawable_desc_set;
        texture_group.sampler = texture_selector.sampler;
        texture_group.textures = tex_vram->Find(texture_selector.textures);
        texture_group.bindings = texture_selector.bindings;

        if (!texture_group.Valid()) {
            continue;
        }

        // Prepares for the draw call.
        if (texture_group.id != kNullUuid) {
            DescriptorSet *desc_set = tex_desc_set_cache->DescriptorSetFor(texture_group);
            vkCmdBindDescriptorSets(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                                    /*firstSet=*/DescriptorSetType::DST_PER_DRAWABLE,
                                    /*descriptorSetCount=*/1, &desc_set->descriptor_set,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        }

        configurator.PushConstant(instance, cmds);

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(cmds, /*firstBinding=*/0, /*bindingCount=*/1,
                               &gpu_geometry.vertex_buffer->buffer,
                               /*pOffsets=*/&offset);
        vkCmdBindIndexBuffer(cmds, gpu_geometry.index_buffer->buffer, /*offset=*/0,
                             gpu_geometry.index_element_type);

        // Draw call.
        vkCmdDrawIndexed(cmds, instance.geometry->indices.index_count,
                         /*instanceCount=*/1, /*firstIndex=*/0, /*vertexOffset=*/0,
                         /*firstInstance=*/0);
    }
}

void PostProcess(GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                 SetPostProcessorUniformsFn const &set_uniforms_fn, VkCommandBuffer cmds) {
    vkCmdBindPipeline(cmds, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    set_uniforms_fn(uniform_layout, cmds);

    vkCmdDraw(cmds, /*vertex_count=*/3, /*instanceCount=*/2, /*firstVertex=*/0,
              /*firstInstance=*/0);
}

} // namespace e8
