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

#ifndef ISLANDS_RENDERER_CONTEXT_H
#define ISLANDS_RENDERER_CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/vma/vk_mem_alloc.h"

namespace e8 {

/**
 * @brief The VulkanContext struct A collection of Vulkan handles usable by all parts of the
 * renderer.
 */
struct VulkanContext {
    VulkanContext();
    ~VulkanContext();

    VkInstance instance;
    VkSurfaceKHR surface;
    std::vector<VkPhysicalDevice> physical_devices;
    VkPhysicalDevice selected_physical_device;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSwapchainKHR swap_chain;
    VkSurfaceFormatKHR swap_chain_image_format;
    VkExtent2D swap_chain_image_extent;
    std::vector<VkImage> swap_chain_images;
    std::vector<VkImageView> swap_chain_image_views;
    VkCommandPool command_pool;
    VmaAllocator allocator;
    VkFence frame_fence;
};

/**
 * @brief CreateVulkanContext Creates a collection of Vulkan handles with resonable configurations
 * usable by all parts of the renderer.
 *
 * @param target_window The target window where the rendering result will be directed to.
 * @return The context to be kept alive throughout the rendering process.
 */
std::unique_ptr<VulkanContext> CreateVulkanContext(SDL_Window *target_window);

} // namespace e8

#endif // ISLANDS_RENDERER_CONTEXT_H
