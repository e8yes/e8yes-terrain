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

#ifndef ISLANDS_RENDERER_H
#define ISLANDS_RENDERER_H

#include <QVulkanWindow>
#include <QVulkanWindowRenderer>

namespace e8 {

/**
 * @brief The IslandsRenderer class Responsible for 3D graphics rendering.
 */
class IslandsRenderer : public QVulkanWindowRenderer {
  public:
    IslandsRenderer(QVulkanWindow *display_window);
    ~IslandsRenderer();

    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;

    void startNextFrame() override;

  private:
    QVulkanWindow *display_window_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_H
