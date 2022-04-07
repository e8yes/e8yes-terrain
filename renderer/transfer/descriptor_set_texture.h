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

#ifndef ISLANDS_RENDERER_DESCRIPTOR_SET_TEXTURE_H
#define ISLANDS_RENDERER_DESCRIPTOR_SET_TEXTURE_H

#include <array>
#include <memory>

#include "renderer/basic/pipeline_common.h"
#include "renderer/transfer/descriptor_set.h"
#include "renderer/transfer/texture_group.h"

namespace e8 {

/**
 * @brief The TextureDescriptorSetCache class A descriptor set cache keyed by texture group. This
 * cache doesn't evict anything automatically but relies on the eviction of texture groups made by
 * external policies.
 */
class TextureDescriptorSetCache {
  public:
    /**
     * @brief TextureDescriptorSetCache Constructs a descriptor set cache upon the specified
     * descriptor set allocator.
     */
    TextureDescriptorSetCache(DescriptorSetAllocator *allocator);
    TextureDescriptorSetCache(TextureDescriptorSetCache const &) = delete;
    ~TextureDescriptorSetCache();

    /**
     * @brief DescriptorSetFor Fetches the descriptor set designated for the specified texture
     * group. If it doesn't exist in the cache, this function creates and prepares it for the group.
     * The descriptor set returned from this function is directly bindable to a pipeline.
     */
    DescriptorSet *DescriptorSetFor(TextureGroup const &texture_group);

    /**
     * @brief OnEvictTextureGroup Notifies the cache that the specified texture group no longer
     * exists. This allows the cache to free up descriptor sets associated with the texture group.
     */
    void OnEvictTextureGroup(TextureGroupId const &texture_group_id);

  private:
    struct TextureDescriptorSetCacheImpl;
    std::unique_ptr<TextureDescriptorSetCacheImpl> pimpl_;
};

} // namespace e8

#endif // ISLANDS_RENDERER_DESCRIPTOR_SET_TEXTURE_H
