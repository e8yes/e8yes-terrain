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
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/sampler.h"

namespace e8 {

ImageSampler::ImageSampler(VulkanContext *context) : context(context) {}

ImageSampler::~ImageSampler() {
    vkDestroySampler(context->device, sampler, /*pAllocator=*/nullptr);
}

std::unique_ptr<ImageSampler> CreateReadBackSampler(VulkanContext *context) {
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

    auto info = std::make_unique<ImageSampler>(context);
    assert(VK_SUCCESS ==
           vkCreateSampler(context->device, &sampler_info, /*pAllocator=*/nullptr, &info->sampler));

    return info;
}

std::unique_ptr<ImageSampler> CreateContinuousSampler(VulkanContext *context) {
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    auto info = std::make_unique<ImageSampler>(context);
    assert(VK_SUCCESS ==
           vkCreateSampler(context->device, &sampler_info, /*pAllocator=*/nullptr, &info->sampler));

    return info;
}

std::unique_ptr<ImageSampler> CreateTextureSampler(VulkanContext *context) {
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = 20.0f;
    sampler_info.anisotropyEnable = VK_TRUE;
    sampler_info.maxAnisotropy = 8;

    auto info = std::make_unique<ImageSampler>(context);
    assert(VK_SUCCESS ==
           vkCreateSampler(context->device, &sampler_info, /*pAllocator=*/nullptr, &info->sampler));

    return info;
}

} // namespace e8
