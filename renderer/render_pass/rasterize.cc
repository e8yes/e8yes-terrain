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

#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/render_pass.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/drawable/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/render_pass/rasterize.h"
#include "renderer/transfer/context.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/texture_group.h"
#include "resource/buffer_texture.h"
#include "resource/common.h"
#include "resource/geometry.h"

namespace e8 {
namespace {

std::unordered_set<DrawableInstance const *>
UploadResources(std::vector<DrawableInstance> const &drawables,
                RenderPassConfiguratorInterface const &configurator,
                TransferContext *transfer_context) {
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

    transfer_context->geometry_vram_transfer.Upload(geometries);
    transfer_context->texture_vram_transfer.Upload(textures);

    return excluded;
}

} // namespace

VkCommandBuffer StartRenderPass(RenderPass const &render_pass, FrameBuffer const &frame_buffer,
                                CommandBuffer *command_buffer) {
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

    vkCmdBeginRenderPass(command_buffer->buffer, &render_pass_begin_info,
                         VK_SUBPASS_CONTENTS_INLINE);

    return command_buffer->buffer;
}

void FinishRenderPass(CommandBuffer *command_buffer) {
    // Finalizes the render pass.
    vkCmdEndRenderPass(command_buffer->buffer);
}

void RenderDrawables(std::vector<DrawableInstance> const &drawables,
                     GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                     RenderPassConfiguratorInterface const &configurator,
                     TransferContext *transfer_context, CommandBuffer *command_buffer) {
    std::unordered_set<DrawableInstance const *> excluded =
        UploadResources(drawables, configurator, transfer_context);

    // Sends drawable instances through the graphics pipeline.
    vkCmdBindPipeline(command_buffer->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
    for (auto const &instance : drawables) {
        if (excluded.find(&instance) != excluded.end()) {
            continue;
        }

        // Fetches GPU geometry data.
        GeometryVramTransfer::GpuGeometry gpu_geometry =
            transfer_context->geometry_vram_transfer.Find(instance.geometry);
        if (!gpu_geometry.Valid()) {
            continue;
        }

        // Fetches GPU texture data.
        TextureSelector texture_selector = configurator.TexturesOf(instance);

        TextureGroup texture_group;
        texture_group.id = texture_selector.id;
        texture_group.layout = uniform_layout.per_drawable_desc_set;
        texture_group.sampler = texture_selector.sampler;
        texture_group.textures =
            transfer_context->texture_vram_transfer.Find(texture_selector.textures);
        texture_group.bindings = texture_selector.bindings;

        if (!texture_group.Valid()) {
            continue;
        }

        // Prepares for the draw call.
        if (texture_group.id != kNullUuid) {
            DescriptorSet *desc_set =
                transfer_context->texture_descriptor_set_cache.DescriptorSetFor(texture_group);
            vkCmdBindDescriptorSets(command_buffer->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    uniform_layout.layout,
                                    /*firstSet=*/DescriptorSetType::DST_PER_DRAWABLE,
                                    /*descriptorSetCount=*/1, &desc_set->descriptor_set,
                                    /*dynamicOffsetCount=*/0,
                                    /*pDynamicOffsets=*/nullptr);
        }

        std::vector<uint8_t> push_constant = configurator.PushConstantOf(instance);
        if (uniform_layout.push_constant_range.has_value() && !push_constant.empty()) {
            vkCmdPushConstants(command_buffer->buffer, uniform_layout.layout,
                               /*stageFlags=*/uniform_layout.push_constant_range->stageFlags,
                               /*offset=*/0, push_constant.size(), push_constant.data());
        }

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(command_buffer->buffer, /*firstBinding=*/0, /*bindingCount=*/1,
                               &gpu_geometry.vertex_buffer->buffer,
                               /*pOffsets=*/&offset);
        vkCmdBindIndexBuffer(command_buffer->buffer, gpu_geometry.index_buffer->buffer,
                             /*offset=*/0, gpu_geometry.index_element_type);

        // Draw call.
        vkCmdDrawIndexed(command_buffer->buffer, instance.geometry->indices.index_count,
                         /*instanceCount=*/1, /*firstIndex=*/0, /*vertexOffset=*/0,
                         /*firstInstance=*/0);
    }
}

void PostProcess(GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                 SetPostProcessorUniformsFn const &set_uniforms_fn, CommandBuffer *command_buffer) {
    vkCmdBindPipeline(command_buffer->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    set_uniforms_fn(uniform_layout, command_buffer);

    vkCmdDraw(command_buffer->buffer, /*vertex_count=*/3, /*instanceCount=*/2, /*firstVertex=*/0,
              /*firstInstance=*/0);
}

} // namespace e8
