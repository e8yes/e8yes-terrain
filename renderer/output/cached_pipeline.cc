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

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "common/device.h"
#include "renderer/basic/fixed_function.h"
#include "renderer/basic/frame_buffer.h"
#include "renderer/basic/pipeline.h"
#include "renderer/basic/sampler.h"
#include "renderer/basic/shader.h"
#include "renderer/basic/uniform_layout.h"
#include "renderer/basic/vertex_input.h"
#include "renderer/output/cached_pipeline.h"
#include "renderer/output/pipeline_output.h"
#include "renderer/output/promise.h"

namespace e8 {

CachedPipelineArgumentsInterface::CachedPipelineArgumentsInterface() {}

CachedPipelineArgumentsInterface::~CachedPipelineArgumentsInterface() {}

CachedPipelineInterface::CachedPipelineInterface(VulkanContext *context) : context_(context) {}

CachedPipelineInterface::~CachedPipelineInterface() {}

} // namespace e8
