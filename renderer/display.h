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

#ifndef ISLANDS_RENDERER_DISPLAY_H
#define ISLANDS_RENDERER_DISPLAY_H

#include <QObject>
#include <QString>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QVulkanWindowRenderer>
#include <memory>

namespace e8 {

/**
 * @brief The RendererDisplay class A Qt-compatible window for displaying rendering result.
 */
class RendererDisplay : public QVulkanWindow {
    Q_OBJECT

  public:
    RendererDisplay();
    ~RendererDisplay();

    QVulkanWindowRenderer *createRenderer() override;
};

/**
 * @brief The RendererContext struct All rendering procedures will be derived from a renderer
 * context.
 */
struct RendererContext {
    // For interacting with the vulkan API.
    std::unique_ptr<QVulkanInstance> vulkan_instance;

    // For displaying the rendering result.
    RendererDisplay *display;
};

/**
 * @brief CreateRendererContext See above for what a renderer context contains.
 */
std::unique_ptr<RendererContext> CreateRendererContext();

} // namespace e8

#endif // ISLANDS_RENDERER_DISPLAY_H
