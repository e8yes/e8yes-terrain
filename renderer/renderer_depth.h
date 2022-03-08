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

#ifndef ISLANDS_RENDERER_DEPTH_H
#define ISLANDS_RENDERER_DEPTH_H

#include <memory>

#include "content/scene.h"
#include "renderer/context.h"

namespace e8 {

/**
 * @brief The DepthRenderer class It renders a depth image at the scene's camera angle.
 */
class DepthRenderer {
  public:
    /**
     * @brief DepthRenderer Constructs a depth renderer.
     *
     * @param context Contextual Vulkan handles.
     */
    DepthRenderer(VulkanContext *context);
    ~DepthRenderer();

    /**
     * @brief DrawFrame Draws a new frame filled with the scene's depth map in grayscale to the
     * currently available swap chain image.
     */
    void DrawFrame(Scene *scene);

  private:
    class DepthRendererImpl;
    std::unique_ptr<DepthRendererImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DEPTH_H
