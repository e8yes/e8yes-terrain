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

#ifndef ISLANDS_RENDERER_MIPMAP_H
#define ISLANDS_RENDERER_MIPMAP_H

#include <vulkan/vulkan.h>

namespace e8 {

/**
 * @brief MipLevelCount Returns a recommended number of mip maps for the given image resolution.
 */
unsigned MipLevelCount(unsigned width, unsigned height);

/**
 * @brief GenerateMipMapFor Generates a mip map chain for the specified image using its level 0 bit
 * map. Note, the generation actually occurs only when the commands are submitted. Also, this
 * function transitions the all mip levels to a different layout. The client of this function may
 * want to transition the layout for all mip levels back after the call.
 *
 * @param image The target image to generate mip map chain for. The image should be created with the
 * mip level count returns by MipLevelCount().
 * @param width The width of the image.
 * @param height The height of the image.
 * @param cmds Stores the mip map generation commands.
 */
void GenerateMipMapFor(VkImage image, unsigned width, unsigned height, VkCommandBuffer cmds);

} // namespace e8

#endif // ISLANDS_RENDERER_MIPMAP_H
