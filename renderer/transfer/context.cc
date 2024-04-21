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

#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/transfer/context.h"
#include "renderer/transfer/vram_geometry.h"
#include "renderer/transfer/vram_texture.h"
#include "renderer/transfer/vram_uniform.h"

namespace e8 {

TransferContext::TransferContext(VulkanContext *context)
    : vulkan_context(context), uniform_vram_transfer(context), geometry_vram_transfer(context),
      texture_vram_transfer(context) {}

TransferContext::~TransferContext() { vkDeviceWaitIdle(vulkan_context->device); }

} // namespace e8
