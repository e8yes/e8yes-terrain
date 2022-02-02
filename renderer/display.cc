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

#include <QByteArrayList>
#include <QObject>
#include <QString>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QVulkanWindowRenderer>
#include <cassert>
#include <memory>

#include "renderer/display.h"
#include "renderer/renderer.h"

namespace e8 {

IslandsRendererDisplay::IslandsRendererDisplay() {}

IslandsRendererDisplay::~IslandsRendererDisplay() {}

QVulkanWindowRenderer *IslandsRendererDisplay::createRenderer() { return new IslandsRenderer(); }

std::unique_ptr<IslandsRendererContext> CreateRendererContext() {
    std::unique_ptr<IslandsRendererContext> context = std::make_unique<IslandsRendererContext>();

    context->vulkan_instance = std::make_unique<QVulkanInstance>();
    context->vulkan_instance->setLayers(QByteArrayList() << "VK_LAYER_GOOGLE_threading"
                                                         << "VK_LAYER_LUNARG_parameter_validation"
                                                         << "VK_LAYER_LUNARG_object_tracker"
                                                         << "VK_LAYER_LUNARG_core_validation"
                                                         << "VK_LAYER_LUNARG_image"
                                                         << "VK_LAYER_LUNARG_swapchain"
                                                         << "VK_LAYER_GOOGLE_unique_objects");
    bool instance_created = context->vulkan_instance->create();
    assert(instance_created);

    context->display = new IslandsRendererDisplay();
    context->display->setVulkanInstance(context->vulkan_instance.get());

    return context;
}

} // namespace e8
