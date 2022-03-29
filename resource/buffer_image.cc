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

#include <boost/log/trivial.hpp>
#include <cassert>
#include <cstdint>
#include <string>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "resource/buffer.h"
#include "resource/buffer_image.h"
#include "resource/common.h"
#include "resource/proto/texture.pb.h"
#include "third_party/stb_image/stb_image.h"

namespace e8 {
namespace {

TextureProto::Encoding OptimalBufferEncoding(TextureProto const & /*texture_proto*/) {
    // TODO: Adds GPU image encoding support.
    return TextureProto::BITMAP;
}

TextureProto::Encoding OptimalProtoEncoding(TextureProto::Encoding /*buffer_encoding*/) {
    // TODO: Adds GPU image encoding support.
    return TextureProto::PNG;
}

void *LoadTexture(TextureProto const &texture_proto) {
    std::string const &image_data = texture_proto.data();

    void *decoded;

    if (texture_proto.channel_size() == sizeof(uint8_t)) {
        int width;
        int height;
        int channel_count;
        decoded = stbi_load_from_memory(reinterpret_cast<stbi_uc const *>(image_data.c_str()),
                                        image_data.length(), &width, &height, &channel_count,
                                        texture_proto.channel_count());
        assert(width == texture_proto.width());
        assert(height == texture_proto.height());
        assert(channel_count == texture_proto.channel_count());
    } else if (texture_proto.channel_size() == sizeof(uint16_t)) {
        int width;
        int height;
        int channel_count;
        decoded = stbi_load_16_from_memory(reinterpret_cast<stbi_uc const *>(image_data.c_str()),
                                           image_data.length(), &width, &height, &channel_count,
                                           texture_proto.channel_count());
        assert(width == texture_proto.width());
        assert(height == texture_proto.height());
        assert(channel_count == texture_proto.channel_count());
    } else {
        BOOST_LOG_TRIVIAL(error) << "LoadTexture(): Unknown channel_size=["
                                 << texture_proto.channel_size() << "].";
        return nullptr;
    }

    if (decoded == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "LoadTexture(): failed to decode texture, reason=["
                                 << stbi_failure_reason() << "].";
        return nullptr;
    }

    return decoded;
}

} // namespace

StagingImageBuffer::StagingImageBuffer() : width(0), height(0), channel_count(0), channel_size(0) {}

StagingImageBuffer::~StagingImageBuffer() {}

void StagingImageBuffer::CreateFromTextureProto(TextureProto const &texture_proto,
                                                VulkanContext *context) {
    void *decoded = LoadTexture(texture_proto);
    if (decoded == nullptr) {
        return;
    }

    encoding = OptimalBufferEncoding(texture_proto);
    width = texture_proto.width();
    height = texture_proto.height();
    channel_count = texture_proto.channel_count();
    channel_size = texture_proto.channel_size();

    this->Allocate(this->ImageSize(), context);

    void *image_buffer = this->BeginAccess();
    memcpy(image_buffer, decoded, this->ImageSize());
    this->EndAccess();

    stbi_image_free(decoded);
}

TextureProto StagingImageBuffer::ToProto() const {
    TextureProto proto;

    if (!this->Valid()) {
        return proto;
    }

    proto.set_encoding(OptimalProtoEncoding(encoding));
    proto.set_width(width);
    proto.set_height(height);
    proto.set_channel_count(channel_count);
    proto.set_channel_size(channel_size);

    void const *source_buffer = this->BeginAccess();
    EncodeTextureData(source_buffer, &proto);
    this->EndAccess();

    return proto;
}

unsigned StagingImageBuffer::ImageSize() const {
    return width * height * channel_count * channel_size;
}

} // namespace e8
