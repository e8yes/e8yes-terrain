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

#ifndef ISLANDS_RESOURCE_BUFFER_TEXTURE_H
#define ISLANDS_RESOURCE_BUFFER_TEXTURE_H

#include "common/device.h"
#include "resource/buffer.h"
#include "resource/proto/texture.pb.h"

namespace e8 {

/**
 * @brief The StagingTextureBuffer class Wraps on a host-only Vulkan image buffer with a simply data
 * conversion layer. The Vulkan texture buffer is typically used for storing a texture image.
 */
class StagingTextureBuffer : public StagingBuffer {
  public:
    StagingTextureBuffer();
    StagingTextureBuffer(StagingTextureBuffer const &) = delete;
    StagingTextureBuffer(StagingTextureBuffer &&) = default;
    ~StagingTextureBuffer();

    StagingTextureBuffer &operator=(StagingTextureBuffer &&) = default;

    /**
     * @brief CreateFromTextureProto Allocates memory for holding the texture and copies texture
     * data into it. If the texture is decodable only through CPU, this function decodes the image
     * data into a bitmap before copying to the allocated memory region.
     */
    void CreateFromTextureProto(TextureProto const &texture_proto, VulkanContext *context);

    /**
     * @brief ToProto Export the image buffer to a protobuf texture. If the image buffer is
     * invalid, it will return an empty texture with invalid encoding. If the image buffer is
     * encoded with GPU encodings, this function keeps those encodings in the export. Otherwise, it
     * always picks the PNG encoding in the exported texture.
     */
    TextureProto ToProto() const;

    /**
     * @brief ImageSize The size of the image, in bytes.
     */
    unsigned ImageSize() const;

  public:
    // The encoding of this texture image.
    TextureProto::Encoding encoding;

    // The width of the texture image.
    unsigned width;

    // The height of the texture image.
    unsigned height;

    // The number of byte each channel uses.
    unsigned channel_count;

    // The number of channel each image pixel carries.
    unsigned channel_size;
};

} // namespace e8

#endif // ISLANDS_RESOURCE_BUFFER_TEXTURE_H
