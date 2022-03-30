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
#include <vector>
#include <vulkan/vulkan.h>

#include "common/cache.h"
#include "common/device.h"
#include "renderer/vram.h"
#include "renderer/vram_texture.h"
#include "resource/buffer_texture.h"

namespace e8 {
namespace {

VkFormat SuitableOneChannelImageFormat(unsigned channel_size) {
    switch (channel_size) {
    case sizeof(uint8_t): {
        return VkFormat::VK_FORMAT_R8_SRGB;
    }
    default: {
        assert(false);
    }
    }
}

VkFormat SuitableThreeChannelsImageFormat(unsigned channel_size) {
    switch (channel_size) {
    case sizeof(uint8_t): {
        return VkFormat::VK_FORMAT_R8G8B8_SRGB;
    }
    case sizeof(uint16_t): {
        return VkFormat::VK_FORMAT_R16G16B16_SNORM;
    }
    default: {
        assert(false);
    }
    }
}

VkFormat SuitableFourChannelsImageFormat(unsigned channel_size) {
    switch (channel_size) {
    case sizeof(uint8_t): {
        return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
    }
    default: {
        assert(false);
    }
    }
}

VkFormat SuitableImageFormat(StagingTextureBuffer const *staging_buffer) {
    // TODO: handles GPU encodings.

    switch (staging_buffer->channel_count) {
    case 1: {
        return SuitableOneChannelImageFormat(staging_buffer->channel_size);
    }
    case 3: {
        return SuitableThreeChannelsImageFormat(staging_buffer->channel_size);
    }
    case 4: {
        return SuitableFourChannelsImageFormat(staging_buffer->channel_size);
    }
    default: {
        assert(false);
    }
    }
}

void UploadStagingTexture(StagingTextureBuffer const *staging_buffer, VulkanContext *context,
                          VramTransfer::GpuTexture *gpu_image, VkCommandBuffer cmds) {
    assert(staging_buffer->Valid());
    assert(staging_buffer->context == context);

    // Creates a memory region to store the image data.
    VkFormat suitable_format = SuitableImageFormat(staging_buffer);
    gpu_image->Allocate(staging_buffer->width, staging_buffer->height, suitable_format, context);

    // Transitions the image's layout to something suitable for data transfer.
    VkImageMemoryBarrier to_transfer_optimal{};
    to_transfer_optimal.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    to_transfer_optimal.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    to_transfer_optimal.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    to_transfer_optimal.image = gpu_image->image;

    to_transfer_optimal.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    to_transfer_optimal.subresourceRange.baseMipLevel = 0;
    to_transfer_optimal.subresourceRange.levelCount = 1;
    to_transfer_optimal.subresourceRange.baseArrayLayer = 0;
    to_transfer_optimal.subresourceRange.layerCount = 1;

    to_transfer_optimal.srcAccessMask = 0;
    to_transfer_optimal.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         /*dstStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT, /*dependencyFlags=*/0,
                         /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr,
                         /*bufferMemoryBarrierCount=*/0, /*pBufferMemoryBarriers=*/nullptr,
                         /*imageMemoryBarrierCount=*/1,
                         /*pImageMemoryBarriers=*/&to_transfer_optimal);

    // Copies the image data.
    VkBufferImageCopy copy_region{};
    copy_region.bufferOffset = 0;
    copy_region.bufferRowLength = 0;
    copy_region.bufferImageHeight = 0;

    copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_region.imageSubresource.mipLevel = 0;
    copy_region.imageSubresource.baseArrayLayer = 0;
    copy_region.imageSubresource.layerCount = 1;
    copy_region.imageExtent.width = staging_buffer->width;
    copy_region.imageExtent.height = staging_buffer->height;
    copy_region.imageExtent.depth = 1;

    vkCmdCopyBufferToImage(cmds, staging_buffer->buffer, gpu_image->image,
                           /*dstImageLayout=*/VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           /*regionCount=*/1, &copy_region);

    // Transitions the image's layout to something samplable by a shader.
    VkImageMemoryBarrier to_shader_readable = to_transfer_optimal;

    to_shader_readable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    to_shader_readable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    to_shader_readable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    to_shader_readable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    // barrier the image into the shader readable layout
    vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
                         /*dstStageMask=*/VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                         /*dependencyFlags=*/0, /*memoryBarrierCount=*/0,
                         /*pMemoryBarriers=*/nullptr, /*bufferMemoryBarrierCount=*/0,
                         /*pBufferMemoryBarriers=*/nullptr, /*imageMemoryBarrierCount=*/1,
                         /*pImageMemoryBarriers=*/&to_shader_readable);
}

void UploadTexture(StagingTextureBuffer const *texture,
                   DeviceCache<StagingTextureBuffer const *, VramTransfer::GpuTexture> *texture_cache,
                   VulkanContext *context, VkCommandBuffer cmds) {
    texture_cache->Upload(
        texture, /*override_old_upload=*/false,
        /*object_size_fn=*/
        [](StagingTextureBuffer const *staging_buffer) { return staging_buffer->ImageSize(); },
        /*upload_fn=*/
        [context, cmds](StagingTextureBuffer const *staging_buffer, uint64_t /*old_object_size*/,
                        uint64_t /*new_object_size*/, VramTransfer::GpuTexture *gpu_image) {
            UploadStagingTexture(staging_buffer, context, gpu_image, cmds);
        });
}

} // namespace

TextureVramTransfer::TextureVramTransfer(VulkanContext *context)
    : VramTransfer(context), texture_cache_(&context->image_vram_usage) {}

TextureVramTransfer::~TextureVramTransfer() {}

void TextureVramTransfer::Prepare() { texture_cache_.Reset(); }

void TextureVramTransfer::Upload(std::vector<StagingTextureBuffer const *> const &textures) {
    VkCommandBuffer cmds = VramTransfer::BeginUpload();

    for (auto texture : textures) {
        UploadTexture(texture, &texture_cache_, context_, cmds);
    }

    VramTransfer::EndUpload(cmds);
}

VramTransfer::GpuTexture *TextureVramTransfer::Find(StagingTextureBuffer const *texture) {
    return texture_cache_.Find(texture);
}

} // namespace e8
