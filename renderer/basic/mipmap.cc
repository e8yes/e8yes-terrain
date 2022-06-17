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
#include <cmath>
#include <vulkan/vulkan.h>

#include "renderer/basic/mipmap.h"

namespace e8 {

unsigned MipLevelCount(unsigned width, unsigned height) {
    return static_cast<unsigned>(std::floor(std::log2(std::max(width, height)))) + 1;
}

void GenerateMipMapFor(VkImage image, unsigned width, unsigned height, VkCommandBuffer cmds) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    unsigned mip_width = width;
    unsigned mip_height = height;
    unsigned mip_levels = MipLevelCount(width, height);

    for (unsigned i = 1; i < mip_levels; i++) {
        // Sets the last mip level to be the source and the current mip level to be the destination.
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
                             /*dstStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT, /*dependencyFlags=*/0,
                             /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr,
                             /*bufferMemoryBarrierCount=*/0, /*pBufferMemoryBarriers=*/nullptr,
                             /*imageMemoryBarrierCount=*/1, &barrier);

        barrier.subresourceRange.baseMipLevel = i;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(
            cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
            /*dstStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT, /*dependencyFlags=*/0,
            /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr, /*bufferMemoryBarrierCount=*/0,
            /*pBufferMemoryBarriers=*/nullptr, /*imageMemoryBarrierCount=*/1, &barrier);

        // Downsamples the the last mip level and writes the result to the current mip level.
        unsigned next_mip_width;
        unsigned next_mip_height;

        if (mip_width > 1) {
            next_mip_width = mip_width >> 1;
        } else {
            next_mip_width = mip_width;
        }

        if (mip_height > 1) {
            next_mip_height = mip_height >> 1;
        } else {
            next_mip_height = mip_height;
        }

        VkImageBlit blit{};

        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1].x = mip_width;
        blit.srcOffsets[1].y = mip_height;
        blit.srcOffsets[1].z = 1;

        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;

        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1].x = next_mip_width;
        blit.dstOffsets[1].y = next_mip_height;
        blit.dstOffsets[1].z = 1;

        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(cmds, /*srcImage=*/image,
                       /*srcImageLayout=*/VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, /*dstImage=*/image,
                       /*dstImageLayout=*/VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, /*regionCount=*/1,
                       /*pRegions=*/&blit, /*filter=*/VK_FILTER_LINEAR);

        mip_width = next_mip_width;
        mip_height = next_mip_height;
    }
}

void AverageValueOf(VkImage image, unsigned width, unsigned height, VkCommandBuffer cmds,
                    VkImage output) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    // Transitions the input image to be source optimal.
    barrier.image = image;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
                         /*dstStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT, /*dependencyFlags=*/0,
                         /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr,
                         /*bufferMemoryBarrierCount=*/0, /*pBufferMemoryBarriers=*/nullptr,
                         /*imageMemoryBarrierCount=*/1, &barrier);

    // Transitions the output image to be destination optimal.
    barrier.image = output;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
                         /*dstStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT, /*dependencyFlags=*/0,
                         /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr,
                         /*bufferMemoryBarrierCount=*/0, /*pBufferMemoryBarriers=*/nullptr,
                         /*imageMemoryBarrierCount=*/1, &barrier);

    // Downsamples (averages) the source image into a 1x1 image.
    VkImageBlit blit{};

    blit.srcOffsets[0] = {0, 0, 0};
    blit.srcOffsets[1].x = width;
    blit.srcOffsets[1].y = height;
    blit.srcOffsets[1].z = 1;

    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.srcSubresource.mipLevel = 0;
    blit.srcSubresource.baseArrayLayer = 0;
    blit.srcSubresource.layerCount = 1;

    blit.dstOffsets[0] = {0, 0, 0};
    blit.dstOffsets[1].x = 1;
    blit.dstOffsets[1].y = 1;
    blit.dstOffsets[1].z = 1;

    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.dstSubresource.mipLevel = 0;
    blit.dstSubresource.baseArrayLayer = 0;
    blit.dstSubresource.layerCount = 1;

    vkCmdBlitImage(cmds, /*srcImage=*/image,
                   /*srcImageLayout=*/VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   /*dstImage=*/output,
                   /*dstImageLayout=*/VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, /*regionCount=*/1,
                   /*pRegions=*/&blit, /*filter=*/VK_FILTER_LINEAR);

    // Transitions the input image to be shader readonly optimal.
    barrier.image = image;

    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
                         /*dstStageMask=*/VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                         /*dependencyFlags=*/0,
                         /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr,
                         /*bufferMemoryBarrierCount=*/0, /*pBufferMemoryBarriers=*/nullptr,
                         /*imageMemoryBarrierCount=*/1, &barrier);

    // Transitions the output image to be shader readonly optimal.
    barrier.image = output;

    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(cmds, /*srcStageMask=*/VK_PIPELINE_STAGE_TRANSFER_BIT,
                         /*dstStageMask=*/VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                         /*dependencyFlags=*/0,
                         /*memoryBarrierCount=*/0, /*pMemoryBarriers=*/nullptr,
                         /*bufferMemoryBarrierCount=*/0, /*pBufferMemoryBarriers=*/nullptr,
                         /*imageMemoryBarrierCount=*/1, &barrier);
}

} // namespace e8
