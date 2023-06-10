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

#ifndef ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_SURFACE_H
#define ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_SURFACE_H

#include <memory>

#include "common/device.h"
#include "renderer/basic/projection.h"
#include "renderer/dag/dag_operation.h"
#include "renderer/drawable/collection.h"
#include "renderer/transfer/context.h"

namespace e8 {

/**
 * @brief The SurfaceProjectionColorOutput enum Index to the surface parameter images (color
 * attachments).
 */
enum SurfaceProjectionColorOutput {
    // The image which stores the normal vector and the roughness factor.
    LICO_NORMAL_ROUGHNESS,

    // The image which stores the albdeo color (in RGB) and the metallic factor.
    LICO_ALBEDO_METALLIC,

    // The number of parameter images.
    LightInputsColorOutputCount,
};

/**
 * @brief CreateProjectSurfaceStage Creates a surface projection pipeline stage with two 32-bit
 * parameter maps and a 32-bit depth map output in the specified dimension. See the above
 * SurfaceProjectionColorOutput enum for what information each parameter map contains.
 *
 * @param width The width of the light parameter map output.
 * @param height The width of the light parameter map output.
 * @param context Contextual Vulkan handles.
 * @return The light input stage.
 */
std::unique_ptr<DagOperation> CreateProjectSurfaceStage(unsigned width, unsigned height,
                                                        VulkanContext *context);

/**
 * @brief DoProjectSurface Schedules a graphics pipeline to generate a map of geometry information.
 * These information are essential for lighting computation. They are: normal vector, roughness
 * factor, albedo, metallic factor and depth.
 *
 * @param drawable_collection A collection of drawables to project to screen space to encode surface
 * information.
 * @param projection Defines how drawables should be projected to the light parameter map.
 * @param transfer_context Transfer context.
 * @param first_stage The frame's first stage.
 * @param target The target stage which stores the rendered light inputs. It should be created using
 * CreateLightInputsStage().
 */
void DoProjectSurface(DrawableCollection *drawable_collection,
                      PerspectiveProjection const &projection, TransferContext *transfer_context,
                      DagOperation *first_stage, DagOperation *target);

} // namespace e8

#endif // ISLANDS_RENDERER_SPACE_PROJECTION_PROJECT_SURFACE_H
