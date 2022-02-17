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

#include <QObject>
#include <QString>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QVulkanWindowRenderer>
#include <memory>
#include <vulkan/vulkan.h>

#include "renderer/vma/vk_mem_alloc.h"

namespace e8 {

/**
 * @brief The IslandsRendererDisplay class A Qt-compatible window for displaying rendering result.
 */
class IslandsRendererDisplay : public QVulkanWindow {
    Q_OBJECT

  public:
    IslandsRendererDisplay();
    ~IslandsRendererDisplay();

    QVulkanWindowRenderer *createRenderer() override;
};

/**
 * @brief The VulkanContext struct A collection of Vulkan handles usable by all parts of the
 * renderer.
 */
struct VulkanContext {
    VulkanContext();
    ~VulkanContext();

    QVulkanInstance instance;
    QVulkanDeviceFunctions *funcs;
    VkDevice device;
    VkCommandPool command_pool;
    VmaAllocator allocator;
};

/**
 * @brief The IslandsRendererContext struct All rendering procedures will be derived from a renderer
 * context.
 */
struct IslandsRendererContext {
    // Stores the global states of Vulkan for this particular renderer.
    QVulkanInstance vulkan_instance;

    // A Qt-compatible window for displaying rendering result. Also, it internally contains a
    // renderer that responds to changes to the camera view and scene view. Note, this context
    // structure doesn't own the display. The client is responsible for deleting the display object.
    IslandsRendererDisplay *display;
};

/**
 * @brief CreateIslandsRendererContext See above for what a renderer context contains.
 */
std::unique_ptr<IslandsRendererContext> CreateIslandsRendererContext();

} // namespace e8

#endif // ISLANDS_RENDERER_CONTEXT_H
