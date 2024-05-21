/**
 * e8yes islands.
 *
 * <p>Copyright (C) 2024 Chifeng Wen {daviesx66@gmail.com}
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
#include <limits>
#include <vector>

#include "renderer/drawable/drawable_instance.h"
#include "renderer/transfer/vram_uniform.h"
#include "resource/material.h"

namespace e8 {

//
unsigned const kNullPackageSlot = std::numeric_limits<unsigned>::max();

/**
 * @brief The UniformPackage class
 */
struct UniformPackage {
    //
    std::vector<StagingUniformBuffer> buffers;

    //
    std::vector<StagingUniformImagePack> texture_packs;

    //
    std::vector<UniformImagePack> image_packs;
};

/**
 * @brief The FrameUniformsInterface class For configuring the shader uniforms pertaining to a
 * frame.
 */
class FrameUniformsInterface {
  public:
    /**
     * @brief FrameUniformsInterface
     */
    FrameUniformsInterface(unsigned package_slot_index);
    virtual ~FrameUniformsInterface();

    /**
     * @brief Uniforms
     * @return
     */
    virtual UniformPackage Uniforms() const;

    /**
     * @brief Empty
     * @return
     */
    static FrameUniformsInterface Empty();

  public:
    unsigned const package_slot_index;
};

/**
 * @brief The RenderPassUniformsInterface class For configuring the shader uniforms going in a
 * render pass
 */
class RenderPassUniformsInterface {
  public:
    /**
     * @brief RenderPassUniformsInterface
     */
    RenderPassUniformsInterface(UniformVramTransfer::TransferId render_pass_id,
                                unsigned package_slot_index);
    virtual ~RenderPassUniformsInterface();

    /**
     * @brief Uniforms
     * @return
     */
    virtual UniformPackage Uniforms() const;

    /**
     * @brief Uniforms
     * @return
     */
    virtual std::vector<uint8_t> UniformPushConstants() const;

    /**
     * @brief Empty
     * @return
     */
    static RenderPassUniformsInterface Empty();

  public:
    UniformVramTransfer::TransferId const render_pass_id;
    unsigned const package_slot_index;
};

/**
 * @brief The MaterialUniformsInterface class For configuring the shader uniform setup applying to
 * each material.
 */
class MaterialUniformsInterface {
  public:
    /**
     * @brief MaterialUniformsInterface
     */
    MaterialUniformsInterface(unsigned package_slot_index);
    virtual ~MaterialUniformsInterface();

    /**
     * @brief UniformsOf
     * @param material
     * @return
     */
    virtual UniformPackage UniformsOf(Material const *material) const;

    /**
     * @brief Empty
     * @return
     */
    static MaterialUniformsInterface Empty();

  public:
    unsigned const package_slot_index;
};

/**
 * @brief The DrawableUniformsInterface class For configuring the shader uniform setup applying to
 * each drawable.
 */
class DrawableUniformsInterface {
  public:
    /**
     * @brief DrawableUniformsInterface
     */
    DrawableUniformsInterface(unsigned package_slot_index);
    virtual ~DrawableUniformsInterface();

    /**
     * @brief UniformsOf
     */
    virtual UniformPackage UniformsOf(DrawableInstance const &drawable) const;

    /**
     * @brief UniformPushConstantsOf
     */
    virtual std::vector<uint8_t> UniformPushConstantsOf(DrawableInstance const &drawable) const;

    /**
     * @brief Empty
     * @return
     */
    static DrawableUniformsInterface Empty();

  public:
    unsigned const package_slot_index;
};

} // namespace e8

#endif // ISLANDS_RENDER_PASS_CONFIGURATOR_H
