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

#include <array>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "renderer/basic/pipeline_common.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/descriptor_set_texture.h"
#include "resource/common.h"

namespace e8 {
namespace {

using DescriptorLayoutToSet =
    std::unordered_map<VkDescriptorSetLayout, std::unique_ptr<DescriptorSet>>;

void WriteDescriptorSet(TextureGroup const &texture_group, DescriptorSet *target) {
    for (unsigned i = 0; i < TEXTURE_TYPE_COUNT; ++i) {
        if (texture_group.textures[i] == nullptr) {
            continue;
        }
        assert(texture_group.bindings[i] >= 0);

        WriteImageDescriptor(texture_group.textures[i]->view, *texture_group.sampler,
                             target->descriptor_set, texture_group.bindings[i],
                             texture_group.textures[i]->context);
    }
}

} // namespace

struct TextureDescriptorSetCache::TextureDescriptorSetCacheImpl {
    TextureDescriptorSetCacheImpl(DescriptorSetAllocator *allocator);
    ~TextureDescriptorSetCacheImpl();

    std::unordered_map<TextureGroupId, DescriptorLayoutToSet> desc_sets;
    DescriptorSetAllocator *allocator;
};

TextureDescriptorSetCache::TextureDescriptorSetCacheImpl::TextureDescriptorSetCacheImpl(
    DescriptorSetAllocator *allocator)
    : allocator(allocator) {
    assert(allocator != nullptr);
}

TextureDescriptorSetCache::TextureDescriptorSetCacheImpl::~TextureDescriptorSetCacheImpl() {}

TextureDescriptorSetCache::TextureDescriptorSetCache(DescriptorSetAllocator *allocator)
    : pimpl_(std::make_unique<TextureDescriptorSetCacheImpl>(allocator)) {}

TextureDescriptorSetCache::~TextureDescriptorSetCache() {}

DescriptorSet *TextureDescriptorSetCache::DescriptorSetFor(TextureGroup const &texture_group) {
    assert(texture_group.id != kNullUuid);

    auto sets_it = pimpl_->desc_sets.find(texture_group.id);
    if (sets_it == pimpl_->desc_sets.end()) {
        sets_it =
            pimpl_->desc_sets.insert(std::make_pair(texture_group.id, DescriptorLayoutToSet()))
                .first;
    }

    auto &[_, by_layouts] = *sets_it;
    auto set_it = by_layouts.find(texture_group.layout);
    if (set_it == by_layouts.end()) {
        std::unique_ptr<DescriptorSet> allocated_desc_set = pimpl_->allocator->Allocate(
            DescriptorType::DT_COMBINED_IMAGE_SAMPLER, texture_group.layout);
        WriteDescriptorSet(texture_group, allocated_desc_set.get());

        set_it =
            by_layouts.insert(std::make_pair(texture_group.layout, std::move(allocated_desc_set)))
                .first;
    }

    auto const &[__, desc_set] = *set_it;
    return desc_set.get();
}

void TextureDescriptorSetCache::OnEvictTextureGroup(TextureGroupId const &texture_group_id) {
    pimpl_->desc_sets.erase(texture_group_id);
}

} // namespace e8
