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

#ifndef ISLANDS_RENDERER_DIRECT_ILLUMINATOR_H
#define ISLANDS_RENDERER_DIRECT_ILLUMINATOR_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "common/device.h"
#include "content/scene_entity.h"
#include "renderer/basic/projection.h"
#include "renderer/output/pipeline_stage.h"
#include "renderer/query/collection.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief The DirectIlluminator class Computes the direct illumination from a list of light sources
 * on a map of lighting parameters.
 */
class DirectIlluminator {
  public:
    /**
     * @brief DirectIlluminator Constructs a direct illuminator with specified resolution.
     *
     * @param width The width of the produced radiance map.
     * @param height The height of the produced radiance map.
     * @param context Contextual Vulkan handles.
     */
    DirectIlluminator(unsigned width, unsigned height, VulkanContext *context);
    ~DirectIlluminator();

    /**
     * @brief DoComputeDirectIllumination Computes the direct illumination from a list of light
     * sources on a map of lighting parameters. Note, it doesn't use the lighting parameter stage
     * when there isn't any light.
     *
     * @param drawable_collection Light sources and geometries for which direct illumination needs
     * to be resolved.
     * @param projected_surface Lighting parameters projected to the screen.
     * @param projection The perspective projection setup which generated the surface parameter
     * map.
     * @param first_stage The frame's first stage.
     * @param transfer_context Transfer context.
     * @return The direct illumination result. Note, the radiance map is cleared to black prior to
     * the direct illumination computation.
     */
    PipelineStage *DoComputeDirectIllumination(DrawableCollection *drawable_collection,
                                               PipelineStage *projected_surface,
                                               PerspectiveProjection const &projection,
                                               PipelineStage *first_stage,
                                               TransferContext *transfer_context);

    /**
     * @brief CachedShadowMaps
     */
    std::unordered_map<SceneEntityId, std::vector<PipelineStage *>> CachedShadowMaps();

  private:
    struct DirectIlluminatorImpl;

    std::unique_ptr<DirectIlluminatorImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DIRECT_ILLUMINATOR_H
