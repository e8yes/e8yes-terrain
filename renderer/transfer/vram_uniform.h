/**
 * e8yes islands.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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

#ifndef ISLANDS_RENDERER_VRAM_UNIFORM_H
#define ISLANDS_RENDERER_VRAM_UNIFORM_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/sampler.h"
#include "renderer/transfer/uniform_promise.h"
#include "resource/buffer_texture.h"

namespace e8 {

/**
 * @brief The StagingUniform class
 */
class StagingUniformBuffer {
  public:
    /**
     * @brief StagingUniformBuffer
     * @param binding
     * @param size
     * @param context
     */
    StagingUniformBuffer(unsigned binding, unsigned size, VulkanContext *context);
    StagingUniformBuffer(StagingUniformBuffer const &) = delete;
    StagingUniformBuffer(StagingUniformBuffer &&);
    ~StagingUniformBuffer();

    /**
     * @brief WriteObject
     * @param uniform_object
     */
    template <typename UniformObject> void WriteObject(UniformObject const &uniform_object) {
        unsigned object_size = sizeof(UniformObject);
        assert(object_size <= max_size_);

        void *dst = this->BeginAccess();
        std::memcpy(dst, &uniform_object, object_size);
        this->EndAccess();

        actual_size_ = object_size;
    }

    /**
     * @brief WriteArray
     * @param uniform_objects
     */
    template <typename UniformObject>
    void WriteArray(std::vector<UniformObject> const &uniform_objects) {
        unsigned objects_size = sizeof(UniformObject) * uniform_objects.size();
        assert(objects_size <= max_size_);

        void *dst = this->BeginAccess();
        std::memcpy(dst, uniform_objects.data(), objects_size);
        this->EndAccess();

        actual_size_ = objects_size;
    }

    /**
     * @brief ActualSize
     * @return
     */
    unsigned ActualSize() const;

  public:
    //
    unsigned const binding;

    //
    VkBuffer buffer;

  private:
    void *BeginAccess();
    void EndAccess();

    unsigned const max_size_;
    unsigned actual_size_;

    VmaAllocation allocation_;
    VulkanContext *context_;
};

/**
 * @brief The StagingUniformImagePack class
 */
struct StagingUniformImagePack {
    StagingUniformImagePack(unsigned binding,
                            std::vector<StagingTextureBuffer const *> const &images,
                            ImageSampler const *image_sampler);
    ~StagingUniformImagePack();

    //
    unsigned const binding;

    //
    std::vector<StagingTextureBuffer const *> images;

    //
    ImageSampler const *image_sampler;
};

/**
 * @brief The UniformImagePack class
 */
struct UniformImagePack {
    UniformImagePack(unsigned binding, std::vector<VkImageView> const &images,
                     ImageSampler const *image_sampler);
    ~UniformImagePack();

    //
    unsigned const binding;

    //
    std::vector<VkImageView> const images;

    //
    ImageSampler const *image_sampler;
};

/**
 * @brief The VramUniformTransfer class
 */
class UniformVramTransfer {
  public:
    //
    using TransferId = int64_t;

    /**
     * @brief VramUniformTransfer
     * @param context
     */
    explicit UniformVramTransfer(VulkanContext *context);
    ~UniformVramTransfer();

    /**
     * @brief Upload
     * @param transfer_id
     * @param buffers
     * @param image_packs
     * @param layout
     */
    void Upload(TransferId const &transfer_id, std::vector<StagingUniformBuffer> const &buffers,
                std::vector<UniformImagePack> const &image_packs, VkDescriptorSetLayout layout);

    /**
     * @brief Find
     * @param transfer_id
     * @return
     */
    GpuUniformPromise *Find(TransferId const &transfer_id);

    /**
     * @brief Evict
     * @param transfer_id
     */
    void Evict(TransferId const &transfer_id);

  private:
    GpuUniformPromiseAllocator allocator_;
    std::unordered_map<TransferId, std::unique_ptr<GpuUniformPromise>> promises_;
    VulkanContext *context_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_VRAM_UNIFORM_H
