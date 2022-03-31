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
#include "renderer/drawable_instance.h"
#include "renderer/pipeline_common.h"
#include "renderer/pipeline_output.h"
#include "renderer/render_pass.h"
#include "renderer/vram_geometry.h"
#include "renderer/vram_texture.h"
#include "resource/buffer_texture.h"
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

        RenderPassConfiguratorInterface::DrawableTextures staging_textures =
            configurator.TexturesOf(drawable);
        if (!staging_textures.Valid()) {
            excluded.insert(&drawable);
            continue;
        }

        geometries.push_back(drawable.geometry);
        staging_textures.ToVector(&textures);
    }

    geo_vram->Upload(geometries);
    tex_vram->Upload(textures);

    return excluded;
}

} // namespace

RenderPassConfiguratorInterface::DrawableTextures::DrawableTextures() {
    for (unsigned i = 0; i < kTextureTypeCount; ++i) {
        staging_textures[i] = nullptr;
    }
}

RenderPassConfiguratorInterface::DrawableTextures::~DrawableTextures() {}

bool RenderPassConfiguratorInterface::DrawableTextures::Valid() const {
    for (unsigned i = 0; i < kTextureTypeCount; ++i) {
        if (staging_textures[i] != nullptr && !staging_textures[i]->Valid()) {
            return false;
        }
    }
    return true;
}

void RenderPassConfiguratorInterface::DrawableTextures::ToVector(
    std::vector<StagingTextureBuffer const *> *textures) const {
    assert(this->Valid());

    for (unsigned i = 0; i < kTextureTypeCount; ++i) {
        if (staging_textures[i] != nullptr) {
            textures->push_back(staging_textures[i]);
        }
    }
}

RenderPassConfiguratorInterface::DrawableGpuTextures::DrawableGpuTextures(
    DrawableTextures const &staging, TextureVramTransfer *tex_vram) {
    assert(staging.Valid());

    for (unsigned i = 0; i < kTextureTypeCount; ++i) {
        if (staging.staging_textures[i] != nullptr) {
            gpu_textures[i] = tex_vram->Find(staging.staging_textures[i]);
        } else {
            gpu_textures[i] = nullptr;
        }
    }
}

RenderPassConfiguratorInterface::DrawableGpuTextures::~DrawableGpuTextures() {}

bool RenderPassConfiguratorInterface::DrawableGpuTextures::Valid() const {
    for (unsigned i = 0; i < kTextureTypeCount; ++i) {
        if (gpu_textures[i] != nullptr && !gpu_textures[i]->Valid()) {
            return false;
        }
    }
    return true;
}

RenderPassConfiguratorInterface::RenderPassConfiguratorInterface() {}

RenderPassConfiguratorInterface::~RenderPassConfiguratorInterface() {}

bool RenderPassConfiguratorInterface::IncludeDrawable(DrawableInstance const & /*drawable*/) const {
    return true;
}

RenderPassConfiguratorInterface::DrawableTextures
RenderPassConfiguratorInterface::TexturesOf(DrawableInstance const & /*drawable*/) const {
    return DrawableTextures();
}

void RenderPassConfiguratorInterface::SetUniformsFor(DrawableInstance const & /*drawable*/,
                                                     DrawableGpuTextures const & /*textures*/,
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

std::unique_ptr<GpuBarrier> FinishRenderPass(VkCommandBuffer cmds, GpuBarrier const &prerequisites,
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

    return std::make_unique<GpuBarrier>(done_signal, cmds, context);
}

void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline,
                     RenderPassConfiguratorInterface const &configurator,
                     GeometryVramTransfer *geo_vram, TextureVramTransfer *tex_vram,
                     VkCommandBuffer cmds) {
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
        RenderPassConfiguratorInterface::DrawableTextures staging_textures =
            configurator.TexturesOf(instance);
        RenderPassConfiguratorInterface::DrawableGpuTextures textures(staging_textures, tex_vram);
        if (!textures.Valid()) {
            continue;
        }

        // Prepares for the draw call.
        configurator.SetUniformsFor(instance, textures, cmds);

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
