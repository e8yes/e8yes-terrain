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

#ifndef ISLANDS_RENDERER_SAMPLER_H
#define ISLANDS_RENDERER_SAMPLER_H

#include <memory>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {

/**
 * @brief The ImageSampler struct For image sampling. It will clean up the sampler resource by the
 * end of its lifecycle.
 */
struct ImageSampler {
    /**
     * @brief ImageSampler Should be created only by calling CreateReadBackSampler().
     */
    ImageSampler(VulkanContext *context);
    ~ImageSampler();

    // A full Vulkan object storing information of a sampler.
    VkSampler sampler;

    // Contextual Vulkan handles.
    VulkanContext *context;
};

/**
 * @brief CreateReadBackSampler Creates an image sampler for the purpose of simply reading back
 * image pixels (no sampling at all).
 *
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ImageSampler structure.
 */
std::unique_ptr<ImageSampler> CreateReadBackSampler(VulkanContext *context);

/**
 * @brief CreateTextureSampler Creates a high quality image sampler for sampling texture images.
 *
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ImageSampler structure.
 */
std::unique_ptr<ImageSampler> CreateTextureSampler(VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_SAMPLER_H
