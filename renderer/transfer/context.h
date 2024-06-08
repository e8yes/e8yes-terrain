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

#ifndef ISLANDS_RENDERER_TRANSFER_CONTEXT_H
#define ISLANDS_RENDERER_TRANSFER_CONTEXT_H

#include "common/device.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "renderer/transfer/vram_uniform.h"

namespace e8 {

/**
 * @brief The TransferContext struct Contains a set of objects for transfering data between the host
 * and the device.
 */
struct TransferContext {
    explicit TransferContext(VulkanContext *context);
    ~TransferContext();

    // Not owned. Contextual Vulkan handles that the transfer objects below depend on.
    VulkanContext *vulkan_context;

    // A generic descriptor set allocator which allows arbitrary number of descriptor set
    // allocations.
    UniformVramTransfer uniform_vram_transfer;

    // It encapsulates an efficient and scalable geometry data transfer from host machine to GPU
    // device via caching.
    GeometryVramTransfer geometry_vram_transfer;

    // It encapsulates an efficient and scalable texture data transfer from host machine to GPU
    // device via caching.
    TextureVramTransfer texture_vram_transfer;
};

} // namespace e8

#endif // ISLANDS_RENDERER_TRANSFER_CONTEXT_H
