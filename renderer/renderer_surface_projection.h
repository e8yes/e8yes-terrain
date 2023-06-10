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

#ifndef ISLANDS_RENDERER_SURFACE_PROJECTION_H
#define ISLANDS_RENDERER_SURFACE_PROJECTION_H

#include <memory>

#include "common/device.h"
#include "content/scene.h"
#include "renderer/proto/renderer.pb.h"
#include "renderer/renderer.h"
#include "resource/accessor.h"

namespace e8 {

/**
 * @brief The SurfaceProjectionRenderer class It visualizes the selected surface parameter
 * (screen-space) as a color image.
 */
class SurfaceProjectionRenderer : public RendererInterface {
  public:
    /**
     * @brief SurfaceProjectionRenderer Constructs a surface projection renderer.
     *
     * @param context Contextual Vulkan handles.
     */
    SurfaceProjectionRenderer(VulkanContext *context);
    ~SurfaceProjectionRenderer();

    void DrawFrame(Scene *scene, ResourceAccessor *resource_accessor) override;

    void ApplyConfiguration(RendererConfiguration const &config) override;

  private:
    class SurfaceProjectionRendererImpl;
    std::unique_ptr<SurfaceProjectionRendererImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_SURFACE_PROJECTION_H
