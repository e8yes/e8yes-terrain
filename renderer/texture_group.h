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

#ifndef ISLANDS_RENDERER_TEXTURE_GROUP_H
#define ISLANDS_RENDERER_TEXTURE_GROUP_H

#include <array>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/pipeline_common.h"
#include "renderer/vram.h"
#include "resource/buffer_texture.h"
#include "resource/common.h"

namespace e8 {

// Uniquely identifies a group of textures.
using TextureGroupId = Uuid;

/**
 * @brief The TextureType enum Enumerates all possible types of texture.
 */
enum TextureType {
    TT_ALBEDO,
    TT_NORMAL,
    TT_METALLIC,
    TT_ROUGHNESS,
    TT_LIGHT_MAP,
    TT_INDIRECT_LIGHT_MAP,
    TEXTURE_TYPE_COUNT,
};

/**
 * @brief The TextureSelector struct Specifies which and how each type of texture is selected.
 */
struct TextureSelector {
    /**
     * @brief TextureSelector Constructs an empty selector with a texture group ID assigned.
     */
    TextureSelector(TextureGroupId const &id);
    ~TextureSelector();

    /**
     * @brief Valid Checks if all non-null textures are valid.
     */
    bool Valid() const;

    /**
     * @brief AppendTo Appends the selected textures the target array.
     */
    void AppendTo(std::vector<StagingTextureBuffer const *> *target) const;

    // ID of the texture group to form from this selector.
    TextureGroupId id;

    // Non-null slots are the selected textures.
    std::array<StagingTextureBuffer const *, TEXTURE_TYPE_COUNT> textures;

    // Non-negative slots are the selected textures' bindings.
    std::array<int, TEXTURE_TYPE_COUNT> bindings;

    // Nullable when the selector is empty. The sampler used for the selected textures.
    ImageSampler const *sampler;
};

/**
 * @brief The TextureGroup struct Represents a logical group of textures that share an instance of
 * descriptor set.
 */
struct TextureGroup {
    /**
     * @brief TextureGroup Constructs an empty texture group.
     */
    TextureGroup();
    ~TextureGroup();

    /**
     * @brief Valid Checks if all non-null textures are valid.
     */
    bool Valid() const;

    // ID of the texture group. This must not be generated. It must be an intrinsic property of this
    // group of textures (i.e. material ID).
    TextureGroupId id;

    // The descriptor set layout to which this group of textures match.
    VkDescriptorSetLayout layout;

    // Non-null slots are the selected textures to form this group.
    std::array<VramTransfer::GpuTexture *, TEXTURE_TYPE_COUNT> textures;

    // Non-negative slots are the selected textures' bindings.
    std::array<int, TEXTURE_TYPE_COUNT> bindings;

    // Nullable when this group is empty. The sampler used for the selected textures.
    ImageSampler const *sampler;
};

} // namespace e8

#endif // ISLANDS_RENDERER_TEXTURE_GROUP_H
