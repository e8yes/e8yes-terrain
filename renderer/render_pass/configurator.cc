/**
 * e8yes islands.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * <p>You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <optional>
#include <vector>

#include "renderer/drawable/drawable_instance.h"
#include "renderer/render_pass/configurator.h"
#include "renderer/transfer/vram_uniform.h"
#include "resource/material.h"

namespace e8 {

UniformsInterface::UniformsInterface(std::optional<unsigned> package_slot_index, bool reuse_upload)
    : package_slot_index(package_slot_index), reuse_upload(reuse_upload) {}

UniformsInterface::UniformsInterface(bool reuse_upload)
    : UniformsInterface(/*package_slot_index=*/std::nullopt, reuse_upload) {}

FrameUniformsInterface::FrameUniformsInterface()
    : UniformsInterface(/*reuse_upload=*/true),
      frame_uniforms_id(UniformVramTransfer::TransferId()) {}

FrameUniformsInterface::FrameUniformsInterface(UniformVramTransfer::TransferId frame_uniforms_id,
                                               unsigned package_slot_index, bool reuse_upload)
    : UniformsInterface(package_slot_index, reuse_upload), frame_uniforms_id(frame_uniforms_id) {}

FrameUniformsInterface::~FrameUniformsInterface() = default;

UniformPackage FrameUniformsInterface::Uniforms() const { return UniformPackage(); }

FrameUniformsInterface FrameUniformsInterface::Empty() { return FrameUniformsInterface(); }

RenderPassUniformsInterface::RenderPassUniformsInterface()
    : UniformsInterface(/*reuse_upload=*/true), render_pass_id(UniformVramTransfer::TransferId()) {}

RenderPassUniformsInterface::RenderPassUniformsInterface(
    UniformVramTransfer::TransferId render_pass_id, std::optional<unsigned> package_slot_index,
    bool reuse_upload)
    : UniformsInterface(package_slot_index, reuse_upload), render_pass_id(render_pass_id) {}

RenderPassUniformsInterface::~RenderPassUniformsInterface() = default;

UniformPackage RenderPassUniformsInterface::Uniforms() const { return UniformPackage(); }

std::vector<uint8_t> RenderPassUniformsInterface::UniformPushConstants() const {
    return std::vector<uint8_t>();
}

RenderPassUniformsInterface RenderPassUniformsInterface::Empty() {
    return RenderPassUniformsInterface();
}

MaterialUniformsInterface::MaterialUniformsInterface() : UniformsInterface(/*reuse_upload=*/true) {}

MaterialUniformsInterface::MaterialUniformsInterface(unsigned package_slot_index, bool reuse_upload)
    : UniformsInterface(package_slot_index, reuse_upload) {}

MaterialUniformsInterface::~MaterialUniformsInterface() = default;

UniformPackage MaterialUniformsInterface::UniformsOf(Material const *) const {
    return UniformPackage();
}

MaterialUniformsInterface MaterialUniformsInterface::Empty() { return MaterialUniformsInterface(); }

DrawableUniformsInterface::DrawableUniformsInterface() : UniformsInterface(/*reuse_upload=*/true) {}

DrawableUniformsInterface::DrawableUniformsInterface(std::optional<unsigned> package_slot_index,
                                                     bool reuse_upload)
    : UniformsInterface(package_slot_index, reuse_upload) {}

DrawableUniformsInterface::~DrawableUniformsInterface() = default;

UniformPackage DrawableUniformsInterface::UniformsOf(DrawableInstance const &) const {
    return UniformPackage();
}

std::vector<uint8_t>
DrawableUniformsInterface::UniformPushConstantsOf(DrawableInstance const &) const {
    return std::vector<uint8_t>();
}

DrawableUniformsInterface DrawableUniformsInterface::Empty() { return DrawableUniformsInterface(); }

} // namespace e8
