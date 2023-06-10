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
#include <cassert>
#include <limits>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/dag/graphics_pipeline_output.h"

namespace e8 {

GraphicsPipelineOutputInterface::GraphicsPipelineOutputInterface(unsigned width, unsigned height)
    : width_(width), height_(height) {}

GraphicsPipelineOutputInterface::~GraphicsPipelineOutputInterface() {}

unsigned GraphicsPipelineOutputInterface::Width() const { return width_; }

unsigned GraphicsPipelineOutputInterface::Height() const { return height_; }

} // namespace e8
