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

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "resource/common.h"
#include "resource/material.h"
#include "resource/proto/material.pb.h"
#include "resource/proto/texture.pb.h"
#include "third_party/stb_image/stb_image.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {
namespace {

std::filesystem::path MaterialFilePath(MaterialId const &id, ResourceTable const &table) {
    std::filesystem::path base_path(table.current_base_path());
    return base_path / (std::to_string(id) + ".mpb");
}

std::filesystem::path MaterialFileName(MaterialProto const &material) {
    return std::to_string(static_cast<MaterialId>(material.id())) + ".mpb";
}

std::filesystem::path MaterialFilePath(std::filesystem::path const &base_path,
                                       MaterialProto const &material) {
    return base_path / MaterialFileName(material);
}

bool LoadTexture(TextureProto const &texture_proto, VulkanContext *context,
                 Material::VulkanImage *image) {
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
        return false;
    }

    if (decoded == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "LoadTexture(): failed to decode texture, reason=["
                                 << stbi_failure_reason() << "].";
        return false;
    }

    image->Allocate(texture_proto.width(), texture_proto.height(), texture_proto.channel_size(),
                    texture_proto.channel_count(), context);

    void *image_buffer = image->BeginAccess();
    memcpy(image_buffer, decoded, image->ImageSize());
    image->EndAccess();

    stbi_image_free(decoded);

    return true;
}

} // namespace

Material::VulkanImage::VulkanImage()
    : buffer(VK_NULL_HANDLE), allocation(VK_NULL_HANDLE), width(0), height(0), channel_size(0),
      channel_count(0), context(VK_NULL_HANDLE) {}

Material::VulkanImage::VulkanImage(VulkanImage &&other) { *this = std::move(other); }

Material::VulkanImage::~VulkanImage() { this->Free(); }

Material::VulkanImage &Material::VulkanImage::operator=(VulkanImage &&other) {
    this->Free();

    buffer = other.buffer;
    allocation = other.allocation;
    width = other.width;
    height = other.height;
    channel_size = other.channel_size;
    channel_count = other.channel_count;
    context = other.context;

    other.buffer = VK_NULL_HANDLE;
    other.allocation = VK_NULL_HANDLE;
    other.context = nullptr;

    return *this;
}

void Material::VulkanImage::Allocate(unsigned width, unsigned height, unsigned channel_size,
                                     unsigned channel_count, VulkanContext *context) {
    this->width = width;
    this->height = height;
    this->channel_size = channel_size;
    this->channel_count = channel_count;
    this->context = context;

    this->Free();

    VkBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = this->ImageSize();
    vertex_buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo allocation_info{};
    allocation_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    vmaCreateBuffer(context->allocator, &vertex_buffer_info, &allocation_info, &buffer, &allocation,
                    /*pAllocationInfo=*/nullptr);
}

void Material::VulkanImage::Free() {
    if (!this->Valid()) {
        return;
    }

    vmaDestroyBuffer(context->allocator, buffer, allocation);
    *this = VulkanImage();
}

void *Material::VulkanImage::BeginAccess() const {
    assert(this->Valid());
    void *data;
    vmaMapMemory(context->allocator, allocation, &data);
    return data;
}

void Material::VulkanImage::EndAccess() const {
    assert(this->Valid());
    vmaUnmapMemory(context->allocator, allocation);
}

bool Material::VulkanImage::Valid() const { return buffer != VK_NULL_HANDLE; }

unsigned Material::VulkanImage::ImageSize() const {
    return width * height * channel_size * channel_count;
}

Material::Material() : id(GenerateUuid()) {}

Material::~Material() {}

void Material::FromDisk(MaterialId const &id, ResourceTable const &table, VulkanContext *context) {
    // Loads protobuf object from disk.
    std::filesystem::path file_path = MaterialFilePath(id, table);
    std::fstream proto_file(file_path, std::ios::in | std::ios::binary);
    assert(proto_file.is_open());

    MaterialProto proto;
    assert(proto.ParseFromIstream(&proto_file));

    // Creates texture maps on Vulkan memory regions.
    LoadTexture(proto.albedo(), context, &albedo);
    LoadTexture(proto.normal(), context, &normal);
    LoadTexture(proto.roughness(), context, &roughness);
}

void SaveMaterialProto(MaterialProto const &material_proto, bool temporary, ResourceTable *table) {
    assert(material_proto.id() != kNullUuid);

    // Writes out the geometry.
    std::filesystem::path file_path = MaterialFilePath(table->current_base_path(), material_proto);
    std::fstream proto_file(file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    assert(proto_file.is_open());
    assert(material_proto.SerializeToOstream(&proto_file));
    proto_file.close();

    // Writes out the metadata for this geometry resource.
    ResourceTable::Metadata metadata;
    metadata.set_file_name(MaterialFileName(material_proto));
    // TODO: This resource size estimate is very inaccurate. Fixes this.
    metadata.set_resource_size(material_proto.ByteSizeLong());
    metadata.set_temporary(temporary);

    (*table->mutable_materials())[material_proto.id()] = metadata;
}

} // namespace e8
