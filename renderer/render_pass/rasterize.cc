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
#include <cstdint>
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
#include "renderer/transfer/uniform_promise.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "renderer/transfer/vram_uniform.h"
#include "resource/buffer_texture.h"
#include "resource/geometry.h"
#include "resource/material.h"

namespace e8 {
namespace {

struct UniformPackageWithLayout {
    UniformPackage uniform_package;
    VkDescriptorSetLayout layout;
};

void UploadGeometries(std::unordered_set<Geometry const *> const &geometries,
                      GeometryVramTransfer *geometry_vram) {
    std::vector<Geometry const *> geo_vec;
    geo_vec.reserve(geometries.size());

    for (auto geometry : geometries) {
        geo_vec.push_back(geometry);
    }

    geometry_vram->Upload(geo_vec);
}

void UploadTextures(std::unordered_map<UniformVramTransfer::TransferId,
                                       UniformPackageWithLayout> const &uniform_packages,
                    TextureVramTransfer *texture_vram) {
    std::vector<StagingTextureBuffer const *> texture_vec;
    texture_vec.reserve(uniform_packages.size());

    for (auto const &[transfer_id, package] : uniform_packages) {
        for (auto const &texture_pack : package.uniform_package.texture_packs) {
            for (auto const &texture_image : texture_pack.images) {
                texture_vec.push_back(texture_image);
            }
        }
    }

    texture_vram->Upload(texture_vec);
}

std::vector<UniformImagePack>
FetchTexturePacks(std::vector<StagingUniformImagePack> const &texture_packs,
                  TextureVramTransfer *texture_vram) {
    std::vector<UniformImagePack> result;
    result.reserve(texture_packs.size());

    for (auto const &staging_texture_pack : texture_packs) {
        std::vector<VramTransfer::GpuTexture *> gpu_textures =
            texture_vram->Find(staging_texture_pack.images);
        std::vector<VkImageView> gpu_image_views;
        gpu_image_views.reserve(gpu_textures.size());
        for (auto gpu_texture : gpu_textures) {
            assert(gpu_texture->Valid());
            gpu_image_views.push_back(gpu_texture->view);
        }

        UniformImagePack gpu_texture_pack(staging_texture_pack.binding, gpu_image_views,
                                          staging_texture_pack.image_sampler);
        result.push_back(gpu_texture_pack);
    }

    return result;
}

void UploadUniforms(std::unordered_map<UniformVramTransfer::TransferId,
                                       UniformPackageWithLayout> const &uniform_packages,
                    TextureVramTransfer *texture_vram, UniformVramTransfer *uniform_vram) {
    for (auto const &[transfer_id, package] : uniform_packages) {
        std::vector<UniformImagePack> image_packs =
            FetchTexturePacks(package.uniform_package.texture_packs, texture_vram);

        uniform_vram->Upload(transfer_id, package.uniform_package.buffers, image_packs,
                             package.layout);
    }
}

void UploadFrameUniforms(FrameUniformsInterface const &frame_uniforms,
                         ShaderUniformLayout const &uniform_layout,
                         UniformVramTransfer *uniform_vram) {
    if (!frame_uniforms.package_slot_index.has_value()) {
        return;
    }
    if (frame_uniforms.reuse_upload &&
        uniform_vram->Find(frame_uniforms.frame_uniforms_id) != nullptr) {
        return;
    }

    UniformPackage uniform_package = frame_uniforms.Uniforms();
    uniform_vram->Upload(frame_uniforms.frame_uniforms_id, uniform_package.buffers,
                         uniform_package.image_packs,
                         uniform_layout.descriptor_set_layouts[*frame_uniforms.package_slot_index]);
}

void UploadRenderPassUniforms(RenderPassUniformsInterface const &render_pass_uniforms,
                              ShaderUniformLayout const &uniform_layout,
                              UniformVramTransfer *uniform_vram) {
    if (!render_pass_uniforms.package_slot_index.has_value()) {
        return;
    }
    if (render_pass_uniforms.reuse_upload &&
        uniform_vram->Find(render_pass_uniforms.render_pass_id) != nullptr) {
        return;
    }

    UniformPackage uniform_package = render_pass_uniforms.Uniforms();
    uniform_vram->Upload(
        render_pass_uniforms.render_pass_id, uniform_package.buffers, uniform_package.image_packs,
        uniform_layout.descriptor_set_layouts[*render_pass_uniforms.package_slot_index]);
}

void UploadResources(std::vector<DrawableInstance> const &drawables,
                     ShaderUniformLayout const &uniform_layout,
                     RenderPassUniformsInterface const &render_pass_uniforms,
                     MaterialUniformsInterface const &material_uniforms,
                     DrawableUniformsInterface const &drawable_uniforms,
                     TransferContext *transfer_context) {
    UploadRenderPassUniforms(render_pass_uniforms, uniform_layout,
                             &transfer_context->uniform_vram_transfer);

    std::unordered_set<Geometry const *> geometries;
    std::unordered_map<UniformVramTransfer::TransferId, UniformPackageWithLayout> uniform_packages;
    geometries.reserve(drawables.size());
    uniform_packages.reserve(2 * drawables.size());

    for (auto const &drawable : drawables) {
        if (drawable.geometry != nullptr) {
            geometries.insert(drawable.geometry);
        }

        if (drawable.material != nullptr && uniform_packages.count(drawable.material->id) == 0 &&
            material_uniforms.package_slot_index.has_value() &&
            (!material_uniforms.reuse_upload ||
             transfer_context->uniform_vram_transfer.Find(drawable.material->id) == nullptr)) {
            UniformPackage material_package = material_uniforms.UniformsOf(drawable.material);
            UniformPackageWithLayout material_package_with_layout{
                .uniform_package = std::move(material_package),
                .layout =
                    uniform_layout.descriptor_set_layouts[*material_uniforms.package_slot_index],
            };
            uniform_packages.insert(
                std::make_pair(drawable.material->id, std::move(material_package_with_layout)));
        }

        if (drawable_uniforms.package_slot_index.has_value() &&
            (!drawable_uniforms.reuse_upload ||
             transfer_context->uniform_vram_transfer.Find(drawable.id) == nullptr)) {
            UniformPackage drawable_package = drawable_uniforms.UniformsOf(drawable);
            UniformPackageWithLayout drawable_package_with_layout{
                .uniform_package = std::move(drawable_package),
                .layout =
                    uniform_layout.descriptor_set_layouts[*drawable_uniforms.package_slot_index],
            };
            uniform_packages.insert(
                std::make_pair(drawable.id, std::move(drawable_package_with_layout)));
        }
    }

    UploadGeometries(geometries, &transfer_context->geometry_vram_transfer);
    UploadTextures(uniform_packages, &transfer_context->texture_vram_transfer);
    UploadUniforms(uniform_packages, &transfer_context->texture_vram_transfer,
                   &transfer_context->uniform_vram_transfer);
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
                     RenderPassUniformsInterface const &render_pass_uniforms,
                     MaterialUniformsInterface const &material_uniforms,
                     DrawableUniformsInterface const &drawable_uniforms,
                     TransferContext *transfer_context, CommandBuffer *cmds) {
    UploadResources(drawables, uniform_layout, render_pass_uniforms, material_uniforms,
                    drawable_uniforms, transfer_context);

    // Sends drawable instances through the graphics pipeline.
    vkCmdBindPipeline(cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    GpuUniformPromise *render_pass_uniform_promise =
        transfer_context->uniform_vram_transfer.Find(render_pass_uniforms.render_pass_id);
    if (render_pass_uniform_promise != nullptr &&
        render_pass_uniforms.package_slot_index.has_value()) {
        vkCmdBindDescriptorSets(
            cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
            /*firstSet=*/*render_pass_uniforms.package_slot_index,
            /*descriptorSetCount=*/1, &render_pass_uniform_promise->descriptor_set,
            /*dynamicOffsetCount=*/0,
            /*pDynamicOffsets=*/nullptr);
    }

    for (auto const &instance : drawables) {
        // Sets uniforms.
        GpuUniformPromise *drawable_uniform_promise =
            transfer_context->uniform_vram_transfer.Find(instance.id);
        if (drawable_uniform_promise != nullptr &&
            drawable_uniforms.package_slot_index.has_value()) {
            vkCmdBindDescriptorSets(
                cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                /*firstSet=*/*drawable_uniforms.package_slot_index,
                /*descriptorSetCount=*/1, &drawable_uniform_promise->descriptor_set,
                /*dynamicOffsetCount=*/0,
                /*pDynamicOffsets=*/nullptr);
        }

        if (instance.material != nullptr) {
            GpuUniformPromise *material_uniform_promise =
                transfer_context->uniform_vram_transfer.Find(instance.material->id);
            if (material_uniform_promise != nullptr &&
                material_uniforms.package_slot_index.has_value()) {
                vkCmdBindDescriptorSets(
                    cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
                    /*firstSet=*/*material_uniforms.package_slot_index,
                    /*descriptorSetCount=*/1, &material_uniform_promise->descriptor_set,
                    /*dynamicOffsetCount=*/0,
                    /*pDynamicOffsets=*/nullptr);
            }
        }

        std::vector<uint8_t> push_constant = drawable_uniforms.UniformPushConstantsOf(instance);
        if (!push_constant.empty()) {
            assert(uniform_layout.push_constant_range.has_value());
            vkCmdPushConstants(cmds->buffer, uniform_layout.layout,
                               /*stageFlags=*/uniform_layout.push_constant_range->stageFlags,
                               /*offset=*/0, push_constant.size(), push_constant.data());
        }

        // Fetches GPU geometry data.
        GeometryVramTransfer::GpuGeometry gpu_geometry =
            transfer_context->geometry_vram_transfer.Find(instance.geometry);
        if (!gpu_geometry.Valid()) {
            continue;
        }

        // Sets geometry.
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(cmds->buffer, /*firstBinding=*/0, /*bindingCount=*/1,
                               &gpu_geometry.vertex_buffer->buffer,
                               /*pOffsets=*/&offset);
        vkCmdBindIndexBuffer(cmds->buffer, gpu_geometry.index_buffer->buffer,
                             /*offset=*/0, gpu_geometry.index_element_type);

        // Draw call.
        vkCmdDrawIndexed(cmds->buffer, instance.geometry->indices.index_count,
                         /*instanceCount=*/1, /*firstIndex=*/0, /*vertexOffset=*/0,
                         /*firstInstance=*/0);
    }
}

void PostProcess(GraphicsPipeline const &pipeline, ShaderUniformLayout const &uniform_layout,
                 FrameUniformsInterface const &frame_uniforms,
                 RenderPassUniformsInterface const &render_pass_uniforms,
                 TransferContext *transfer_context, CommandBuffer *cmds) {
    UploadFrameUniforms(frame_uniforms, uniform_layout, &transfer_context->uniform_vram_transfer);
    UploadRenderPassUniforms(render_pass_uniforms, uniform_layout,
                             &transfer_context->uniform_vram_transfer);

    vkCmdBindPipeline(cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    GpuUniformPromise *frame_uniform_promise =
        transfer_context->uniform_vram_transfer.Find(frame_uniforms.frame_uniforms_id);
    if (frame_uniform_promise != nullptr && frame_uniforms.package_slot_index.has_value()) {
        vkCmdBindDescriptorSets(cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                uniform_layout.layout,
                                /*firstSet=*/*frame_uniforms.package_slot_index,
                                /*descriptorSetCount=*/1, &frame_uniform_promise->descriptor_set,
                                /*dynamicOffsetCount=*/0,
                                /*pDynamicOffsets=*/nullptr);
    }

    GpuUniformPromise *render_pass_uniform_promise =
        transfer_context->uniform_vram_transfer.Find(render_pass_uniforms.render_pass_id);
    if (render_pass_uniform_promise != nullptr &&
        render_pass_uniforms.package_slot_index.has_value()) {
        vkCmdBindDescriptorSets(
            cmds->buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, uniform_layout.layout,
            /*firstSet=*/*render_pass_uniforms.package_slot_index,
            /*descriptorSetCount=*/1, &render_pass_uniform_promise->descriptor_set,
            /*dynamicOffsetCount=*/0,
            /*pDynamicOffsets=*/nullptr);
    }

    std::vector<uint8_t> push_constant = render_pass_uniforms.UniformPushConstants();
    if (!push_constant.empty()) {
        assert(uniform_layout.push_constant_range.has_value());
        vkCmdPushConstants(cmds->buffer, uniform_layout.layout,
                           /*stageFlags=*/uniform_layout.push_constant_range->stageFlags,
                           /*offset=*/0, push_constant.size(), push_constant.data());
    }

    vkCmdDraw(cmds->buffer, /*vertex_count=*/3, /*instanceCount=*/2, /*firstVertex=*/0,
              /*firstInstance=*/0);
}

} // namespace e8
