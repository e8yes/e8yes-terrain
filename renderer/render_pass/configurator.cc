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

#include "renderer/render_pass/configurator.h"
#include "renderer/transfer/vram_uniform.h"

namespace e8 {

RenderPassUniformsInterface::RenderPassUniformsInterface(
    UniformVramTransfer::TransferId render_pass_id, unsigned package_slot_index)
    : render_pass_id(render_pass_id), package_slot_index(package_slot_index) {}

RenderPassUniformsInterface::~RenderPassUniformsInterface() = default;

MaterialUniformsInterface::MaterialUniformsInterface(unsigned package_slot_index)
    : package_slot_index(package_slot_index) {}

MaterialUniformsInterface::~MaterialUniformsInterface() = default;

DrawableUniformsInterface::DrawableUniformsInterface(unsigned package_slot_index)
    : package_slot_index(package_slot_index) {}

DrawableUniformsInterface::~DrawableUniformsInterface() = default;

} // namespace e8
