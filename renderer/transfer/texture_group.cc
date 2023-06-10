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
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/basic/sampler.h"
#include "renderer/transfer/texture_group.h"
#include "renderer/transfer/vram.h"
#include "resource/buffer_texture.h"

namespace e8 {

TextureSelector::TextureSelector(TextureGroupId const &id) : id(id), sampler(nullptr) {
    for (unsigned i = 0; i < TEXTURE_TYPE_COUNT; ++i) {
        textures[i] = nullptr;
        bindings[i] = -1;
    }
}

TextureSelector::~TextureSelector() {}

bool TextureSelector::Valid() const {
    for (unsigned i = 0; i < TEXTURE_TYPE_COUNT; ++i) {
        if (textures[i] != nullptr) {
            assert(bindings[i] != -1);

            if (!textures[i]->Valid()) {
                return false;
            }
        }
    }
    return true;
}

void TextureSelector::AppendTo(std::vector<StagingTextureBuffer const *> *target) const {
    assert(this->Valid());

    for (unsigned i = 0; i < TEXTURE_TYPE_COUNT; ++i) {
        if (textures[i] != nullptr) {
            target->push_back(textures[i]);
        }
    }
}

TextureGroup::TextureGroup() {}

TextureGroup::~TextureGroup() {}

bool TextureGroup::Valid() const {
    for (unsigned i = 0; i < TEXTURE_TYPE_COUNT; ++i) {
        if (textures[i] != nullptr) {
            assert(bindings[i] != -1);

            if (!textures[i]->Valid()) {
                return false;
            }
        }
    }
    return true;
}

} // namespace e8
