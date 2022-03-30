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

#ifndef ISLANDS_RENDERER_VRAM_IMAGE_H
#define ISLANDS_RENDERER_VRAM_IMAGE_H

#include <vector>

#include "common/cache.h"
#include "common/device.h"
#include "renderer/vram.h"
#include "resource/buffer_texture.h"

namespace e8 {

/**
 * @brief The TextureVramTransfer class It encapsulates an efficient and scalable texture data
 * transfer from host machine to GPU device via caching. This class isn't thread-safe.
 */
class TextureVramTransfer : public VramTransfer {
  public:
    TextureVramTransfer(VulkanContext *context);
    ~TextureVramTransfer();

    /**
     * @brief Prepare Calls this when all the uploaded GPU images are no longer needed, and
     * therefore, are allowed to be ejected.
     */
    void Prepare();

    /**
     * @brief Upload Transfers a list of staging images to the GPU device for those that have not
     * been there. This function blocks until the uploads, if there are any, are complete.
     *
     * @param images The images to be uploaded.
     */
    void Upload(std::vector<StagingTextureBuffer const *> const &textures);

    /**
     * @brief Find Returns the uploaded image. If the specified image has not been uploaded prior to
     * this call, this function will fail.
     */
    GpuTexture *Find(StagingTextureBuffer const *texture);

  private:
    DeviceCache<StagingTextureBuffer const *, GpuTexture> texture_cache_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_VRAM_IMAGE_H
