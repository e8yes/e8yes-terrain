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

#include <cstdint>
#include <vector>

#include "renderer/query/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/transfer/texture_group.h"
#include "resource/common.h"

namespace e8 {

RenderPassConfiguratorInterface::RenderPassConfiguratorInterface() {}

RenderPassConfiguratorInterface::~RenderPassConfiguratorInterface() {}

bool RenderPassConfiguratorInterface::IncludeDrawable(DrawableInstance const & /*drawable*/) const {
    return true;
}

std::vector<uint8_t>
RenderPassConfiguratorInterface::PushConstantOf(DrawableInstance const & /*drawable*/) const {
    return std::vector<uint8_t>();
}

TextureSelector
RenderPassConfiguratorInterface::TexturesOf(DrawableInstance const & /*drawable*/) const {
    return TextureSelector(kNullUuid);
}

} // namespace e8
