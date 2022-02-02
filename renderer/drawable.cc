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

#include <string>
#include <vector>

#include "common/tensor.h"
#include "renderer/drawable.h"

namespace e8 {

IslandsDrawableInterface::IslandsDrawableInterface(std::string const &human_readable_name)
    : human_readable_name_(human_readable_name) {}

IslandsDrawableInterface::~IslandsDrawableInterface() {}

std::string IslandsDrawableInterface::HumanReadableName() const { return human_readable_name_; }

} // namespace e8