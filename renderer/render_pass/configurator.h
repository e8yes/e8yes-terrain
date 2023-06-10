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

#ifndef ISLANDS_RENDER_PASS_CONFIGURATOR_H
#define ISLANDS_RENDER_PASS_CONFIGURATOR_H

#include <cstdint>
#include <vector>

#include "renderer/query/drawable_instance.h"
#include "renderer/transfer/texture_group.h"

namespace e8 {

/**
 * @brief The RenderPassConfiguratorInterface class For configuring what resources go in a render
 * pass and what shader uniform setup to apply to each drawable.
 */
class RenderPassConfiguratorInterface {
  public:
    RenderPassConfiguratorInterface();
    virtual ~RenderPassConfiguratorInterface();

    /**
     * @brief IncludeDrawable Whether to include the specified drawable to the render pass?
     */
    virtual bool IncludeDrawable(DrawableInstance const &drawable) const;

    /**
     * @brief PushConstantOf A byte array containing push constant data of the specified drawable.
     */
    virtual std::vector<uint8_t> PushConstantOf(DrawableInstance const &drawable) const;

    /**
     * @brief TexturesOf Textures of the specified drawable needed by the render pass.
     */
    virtual TextureSelector TexturesOf(DrawableInstance const &drawable) const;
};

} // namespace e8

#endif // ISLANDS_RENDER_PASS_CONFIGURATOR_H
