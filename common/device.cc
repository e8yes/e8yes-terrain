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

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"

namespace e8 {
namespace {

using SdlExtensions = std::vector<char const *>;
using DeviceExtensions = std::vector<char const *>;
using ValidationLayers = std::vector<char const *>;
using QueueFamilyIndex = unsigned;

constexpr char const *kAppName = "e8 islands";
constexpr char const *kEngineName = "e8 islands rendererz";
uint32_t const kVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
uint32_t const kMinimumVulkanApiVersion = VK_API_VERSION_1_2;

ValidationLayers const kDebugModeValidationLayers{
    //"VK_LAYER_VALVE_steam_overlay_32",
    // "VK_LAYER_VALVE_steam_fossilize_32",
    // "VK_LAYER_VALVE_steam_overlay_64",
    // "VK_LAYER_VALVE_steam_fossilize_64",
    //"VK_LAYER_MESA_device_select",
    // "VK_LAYER_LUNARG_screenshot",
    // "VK_LAYER_LUNARG_monitor",
    // "VK_LAYER_LUNARG_gfxreconstruct",
    // "VK_LAYER_LUNARG_api_dump",
    // "VK_LAYER_KHRONOS_synchronization2",
    // "VK_LAYER_KHRONOS_profiles",
    "VK_LAYER_KHRONOS_validation",
    // "VK_LAYER_LUNARG_device_simulation",
    // "VK_LAYER_MESA_overlay"
};

SdlExtensions RequiredExtensions(SDL_Window *target_window) {
    unsigned extension_count = 0;
    SDL_Vulkan_GetInstanceExtensions(target_window, &extension_count, nullptr);

    SdlExtensions extensions(extension_count);
    SDL_Vulkan_GetInstanceExtensions(target_window, &extension_count, extensions.data());

    return extensions;
}

VkInstance CreateVulkanInstance(SDL_Window *target_window) {
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = kAppName;
    app_info.applicationVersion = kVersion;
    app_info.pEngineName = kEngineName;
    app_info.engineVersion = kVersion;
    app_info.apiVersion = kMinimumVulkanApiVersion;

    VkInstanceCreateInfo instance_info{};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &app_info;
    instance_info.ppEnabledLayerNames = kDebugModeValidationLayers.data();
    instance_info.enabledLayerCount = kDebugModeValidationLayers.size();

    SdlExtensions sdl_extensions = RequiredExtensions(target_window);
    instance_info.ppEnabledExtensionNames = sdl_extensions.data();
    instance_info.enabledExtensionCount = sdl_extensions.size();

    VkInstance instance;
    assert(VK_SUCCESS == vkCreateInstance(&instance_info, /*pAllocator=*/nullptr, &instance));

    return instance;
}

std::vector<VkPhysicalDevice> EnumeratePhysicalDevices(VkInstance instance) {
    unsigned physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

    return physical_devices;
}

VkPhysicalDevice
SelectSuitablePhysicalDevice(std::vector<VkPhysicalDevice> const &physical_devices) {
    std::unordered_map<VkPhysicalDeviceType, VkPhysicalDevice> suitable_physical_devices;

    for (auto physical_device : physical_devices) {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        vkGetPhysicalDeviceFeatures(physical_device, &features);

        auto it = suitable_physical_devices.find(properties.deviceType);
        if (it != suitable_physical_devices.end()) {
            // Accepts only one suitable device per device type.
            continue;
        }

        if (properties.apiVersion >= kMinimumVulkanApiVersion) {
            // TODO: Checks feature prerequisites as well.
            suitable_physical_devices.insert(
                std::make_pair(properties.deviceType, physical_device));
        }
    }

    assert(!suitable_physical_devices.empty());

    auto it =
        suitable_physical_devices.find(VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    if (it != suitable_physical_devices.end()) {
        return it->second;
    }

    it = suitable_physical_devices.find(
        VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
    if (it != suitable_physical_devices.end()) {
        return it->second;
    }

    it = suitable_physical_devices.find(VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU);
    if (it != suitable_physical_devices.end()) {
        return it->second;
    }

    it = suitable_physical_devices.find(VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER);
    if (it != suitable_physical_devices.end()) {
        return it->second;
    }

    it = suitable_physical_devices.find(VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU);
    if (it != suitable_physical_devices.end()) {
        return it->second;
    }

    // Unknown device type.
    assert(false);
}

std::vector<VkQueueFamilyProperties> QueueFamilyProperties(VkPhysicalDevice physical_device) {
    unsigned queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             /*pQueueFamilyProperties=*/nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             queue_family_properties.data());

    return queue_family_properties;
}

QueueFamilyIndex FindGraphicsQueueFamilyIndex(VkPhysicalDevice physical_device) {
    std::vector<VkQueueFamilyProperties> queue_family_properties =
        QueueFamilyProperties(physical_device);

    auto graphics_queue_it = std::find_if(
        queue_family_properties.begin(), queue_family_properties.end(),
        [](VkQueueFamilyProperties properties) {
            return properties.queueCount > 0 && properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
        });
    assert(graphics_queue_it != queue_family_properties.end());
    return std::distance(queue_family_properties.begin(), graphics_queue_it);
}

QueueFamilyIndex FindPresentQueueFamilyIndex(VkPhysicalDevice physical_device,
                                             VkSurfaceKHR surface) {
    std::vector<VkQueueFamilyProperties> queue_family_properties =
        QueueFamilyProperties(physical_device);

    for (QueueFamilyIndex queue_family_index = 0;
         queue_family_index < queue_family_properties.size(); ++queue_family_index) {
        VkBool32 support_present;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, surface,
                                             &support_present);
        if (support_present) {
            return queue_family_index;
        }
    }

    assert(false);
}

VkDevice CreateDevice(VkPhysicalDevice physical_device,
                      QueueFamilyIndex graphics_queue_family_index,
                      QueueFamilyIndex present_queue_family_index, VkQueue *graphics_queue,
                      VkQueue *present_queue) {
    DeviceExtensions device_extensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME,
    };

    std::vector<VkDeviceQueueCreateInfo> queue_infos;
    std::vector<QueueFamilyIndex> required_queues{graphics_queue_family_index};
    if (present_queue_family_index != graphics_queue_family_index) {
        required_queues.push_back(present_queue_family_index);
    }

    float queue_priority = 1.0f;
    for (QueueFamilyIndex queueFamily : required_queues) {
        VkDeviceQueueCreateInfo queue_info{};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex = queueFamily;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &queue_priority;
        queue_infos.push_back(queue_info);
    }

    VkPhysicalDeviceFeatures device_features = {};
    device_features.depthBiasClamp = VK_TRUE;
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo device_info{};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pQueueCreateInfos = queue_infos.data();
    device_info.queueCreateInfoCount = queue_infos.size();
    device_info.pEnabledFeatures = &device_features;
    device_info.enabledExtensionCount = device_extensions.size();
    device_info.ppEnabledExtensionNames = device_extensions.data();

    VkDevice device;
    assert(VK_SUCCESS ==
           vkCreateDevice(physical_device, &device_info, /*pAllocator=*/nullptr, &device));

    vkGetDeviceQueue(device, graphics_queue_family_index, /*queueIndex=*/0, graphics_queue);
    vkGetDeviceQueue(device, present_queue_family_index, /*queueIndex=*/0, present_queue);

    return device;
}

VkSurfaceFormatKHR SelectOptimalSwapChainImageFormat(VkPhysicalDevice physical_device,
                                                     VkSurfaceKHR surface) {
    unsigned format_count;
    assert(VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface,
                                                              &format_count,
                                                              /*pSurfaceFormats=*/nullptr));
    std::vector<VkSurfaceFormatKHR> available_formats(format_count);
    assert(VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR(
                             physical_device, surface, &format_count, available_formats.data()));
    assert(!available_formats.empty());

    for (auto available_format : available_formats) {
        if (available_format.format == VK_FORMAT_R8G8B8A8_SRGB &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return available_format;
        }
    }

    return available_formats.front();
}

VkSwapchainKHR CreateSwapChain(VkDevice device, VkPhysicalDevice physical_device,
                               VkSurfaceKHR surface, QueueFamilyIndex graphics_queue,
                               QueueFamilyIndex present_queue,
                               VkSurfaceFormatKHR *swap_chain_image_format,
                               VkExtent2D *swap_chain_image_extent,
                               std::vector<VkImage> *swap_chain_images) {
    VkSurfaceCapabilitiesKHR capabilities;
    assert(VK_SUCCESS ==
           vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities));

    *swap_chain_image_format = SelectOptimalSwapChainImageFormat(physical_device, surface);
    *swap_chain_image_extent = capabilities.currentExtent;

    VkSwapchainCreateInfoKHR swap_chain_info{};
    swap_chain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_info.surface = surface;
    swap_chain_info.minImageCount = capabilities.minImageCount + 1;
    swap_chain_info.imageFormat = swap_chain_image_format->format;
    swap_chain_info.imageColorSpace = swap_chain_image_format->colorSpace;
    swap_chain_info.imageExtent = *swap_chain_image_extent;
    swap_chain_info.imageArrayLayers = 1;
    swap_chain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    std::vector<QueueFamilyIndex> queue_family_indices{graphics_queue, present_queue};
    if (present_queue == graphics_queue) {
        swap_chain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    } else {
        swap_chain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swap_chain_info.pQueueFamilyIndices = queue_family_indices.data();
        swap_chain_info.queueFamilyIndexCount = queue_family_indices.size();
    }

    swap_chain_info.preTransform = capabilities.currentTransform;
    swap_chain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swap_chain_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swap_chain_info.clipped = VK_TRUE;

    VkSwapchainKHR swap_chain;
    assert(VK_SUCCESS ==
           vkCreateSwapchainKHR(device, &swap_chain_info, /*pAllocator=*/nullptr, &swap_chain));

    unsigned image_count;
    assert(VK_SUCCESS ==
           vkGetSwapchainImagesKHR(device, swap_chain, &image_count, /*pSwapchainImages=*/nullptr));
    swap_chain_images->resize(image_count);
    assert(VK_SUCCESS ==
           vkGetSwapchainImagesKHR(device, swap_chain, &image_count, swap_chain_images->data()));

    return swap_chain;
}

std::vector<VkImageView> CreateSwapChainImageViews(VkDevice device,
                                                   VkFormat swap_chain_image_format,
                                                   std::vector<VkImage> const &swap_chain_images) {
    std::vector<VkImageView> swap_chain_image_views;

    std::transform(swap_chain_images.begin(), swap_chain_images.end(),
                   std::back_inserter(swap_chain_image_views),
                   [device, swap_chain_image_format](VkImage swap_chain_image) {
                       VkImageViewCreateInfo image_view_info{};
                       image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                       image_view_info.image = swap_chain_image;
                       image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
                       image_view_info.format = swap_chain_image_format;
                       image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                       image_view_info.subresourceRange.baseMipLevel = 0;
                       image_view_info.subresourceRange.levelCount = 1;
                       image_view_info.subresourceRange.baseArrayLayer = 0;
                       image_view_info.subresourceRange.layerCount = 1;

                       VkImageView swap_chain_image_view;
                       assert(VK_SUCCESS == vkCreateImageView(device, &image_view_info,
                                                              /*pAllocator=*/nullptr,
                                                              &swap_chain_image_view));

                       return swap_chain_image_view;
                   });

    return swap_chain_image_views;
}

VkCommandPool CreateCommandPool(VkDevice device, QueueFamilyIndex graphics_queue) {
    VkCommandPoolCreateInfo command_pool_info{};
    command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_info.queueFamilyIndex = graphics_queue;

    VkCommandPool command_pool;
    assert(VK_SUCCESS ==
           vkCreateCommandPool(device, &command_pool_info, /*pAllocator=*/nullptr, &command_pool));

    return command_pool;
}

VmaAllocator CreateVmaAllocator(VkInstance instance, VkPhysicalDevice selected_physical_device,
                                VkDevice device) {
    VmaAllocatorCreateInfo allocator_info = {};
    allocator_info.instance = instance;
    allocator_info.physicalDevice = selected_physical_device;
    allocator_info.device = device;

    VmaAllocator allocator;
    assert(VK_SUCCESS == vmaCreateAllocator(&allocator_info, &allocator));

    return allocator;
}

} // namespace

MemoryUsageTracker::MemoryUsageTracker(uint64_t capacity) : used(0), capacity(capacity) {}

MemoryUsageTracker::~MemoryUsageTracker() {}

VulkanContext::VulkanContext()
    : instance(VK_NULL_HANDLE), selected_physical_device(VK_NULL_HANDLE), surface(VK_NULL_HANDLE),
      device(VK_NULL_HANDLE), graphics_queue(VK_NULL_HANDLE), present_queue(VK_NULL_HANDLE),
      swap_chain(VK_NULL_HANDLE), swap_chain_image_format{}, swap_chain_image_extent{},
      command_pool(VK_NULL_HANDLE), allocator(VK_NULL_HANDLE) {}

VulkanContext::~VulkanContext() {
    vmaDestroyAllocator(allocator);
    vkDestroyCommandPool(device, command_pool, /*pAllocator=*/nullptr);
    for (auto swap_chain_image_view : swap_chain_image_views) {
        vkDestroyImageView(device, swap_chain_image_view, /*pAllocator=*/nullptr);
    }
    vkDestroySwapchainKHR(device, swap_chain, /*pAllocator=*/nullptr);
    vkDestroyDevice(device, /*pAllocator=*/nullptr);
    vkDestroySurfaceKHR(instance, surface, /*pAllocator=*/nullptr);
    vkDestroyInstance(instance, /*pAllocator=*/nullptr);
}

std::unique_ptr<VulkanContext> CreateVulkanContext(SDL_Window *target_window) {
    auto context = std::make_unique<VulkanContext>();

    context->instance = CreateVulkanInstance(target_window);

    context->physical_devices = EnumeratePhysicalDevices(context->instance);
    context->selected_physical_device = SelectSuitablePhysicalDevice(context->physical_devices);

    assert(SDL_TRUE ==
           SDL_Vulkan_CreateSurface(target_window, context->instance, &context->surface));

    QueueFamilyIndex graphics_queue_family_index =
        FindGraphicsQueueFamilyIndex(context->selected_physical_device);
    QueueFamilyIndex present_queue_family_index =
        FindPresentQueueFamilyIndex(context->selected_physical_device, context->surface);

    context->device =
        CreateDevice(context->selected_physical_device, graphics_queue_family_index,
                     present_queue_family_index, &context->graphics_queue, &context->present_queue);

    context->swap_chain = CreateSwapChain(
        context->device, context->selected_physical_device, context->surface,
        graphics_queue_family_index, present_queue_family_index, &context->swap_chain_image_format,
        &context->swap_chain_image_extent, &context->swap_chain_images);
    context->swap_chain_image_views = CreateSwapChainImageViews(
        context->device, context->swap_chain_image_format.format, context->swap_chain_images);

    context->command_pool = CreateCommandPool(context->device, graphics_queue_family_index);

    context->allocator =
        CreateVmaAllocator(context->instance, context->selected_physical_device, context->device);

    return context;
}

} // namespace e8
