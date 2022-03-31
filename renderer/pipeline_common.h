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

#ifndef ISLANDS_RENDERER_PIPELINE_COMMON_H
#define ISLANDS_RENDERER_PIPELINE_COMMON_H

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "common/device.h"
#include "third_party/vma/vk_mem_alloc.h"

namespace e8 {

// File paths to the shader programs.
constexpr char const *kVertexShaderFilePathDepthMap = "./depth.vert.spv";
constexpr char const *kVertexShaderFilePathLightInputs = "./light_inputs.vert.spv";
constexpr char const *kVertexShaderFilePathPostProcessor = "./post_processor.vert.spv";
constexpr char const *kFragmentShaderFilePathDepthMapVisualizer = "./depth_visualizer.frag.spv";
constexpr char const *kFragmentShaderFilePathLightInputs = "./light_inputs.frag.spv";
constexpr char const *kFragmentShaderFilePathPostProcessorEmpty = "./post_processor.frag.spv";

/**
 * @brief The ShaderStages struct Stores the shader handle and stage information of a graphics
 * pipeline. It will clean up shader resources by the end of its lifecycle.
 */
struct ShaderStages {
    /**
     * @brief ShaderStages Should be created only by calling CreateShaderStages().
     */
    explicit ShaderStages(VulkanContext *context);
    ~ShaderStages();

    ShaderStages(ShaderStages const &) = delete;
    ShaderStages(ShaderStages &&) = delete;

    // Shader modules loaded and the stage information attached.
    std::vector<VkShaderModule> modules;
    std::vector<VkPipelineShaderStageCreateInfo> stages;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateShaderStages Loads vertex and fragment shader programs in SPIR-V IR and prepares
 * them for a graphics pipeline. This function will always return a valid ShaderStages structure.
 * Any failure occurs in the process of loading shader programs will make it fail.
 *
 * @param vertex_shader_file_path File path to the vertex shader program. Note, vertex shader is a
 * required shader stage.
 * @param fragment_shader_file_path File path to the fragment shader program. This shader stage is
 * optional.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ShaderStages structure.
 */
std::unique_ptr<ShaderStages>
CreateShaderStages(std::string const &vertex_shader_file_path,
                   std::optional<std::string> const &fragment_shader_file_path,
                   VulkanContext *context);

/**
 * @brief The ShaderUniformLayoutInfo struct Stores a Vulkan object describing the layout of the
 * uniform variables declared in the shader programs in a graphics pipeline. It will clean up layout
 * resources by the end of its lifecycle.
 */
struct ShaderUniformLayout {
    /**
     * @brief ShaderUniformLayoutInfo Should be created only by calling CreateShaderUniformLayout().
     */
    explicit ShaderUniformLayout(VulkanContext *context);
    ~ShaderUniformLayout();

    ShaderUniformLayout(ShaderUniformLayout const &) = delete;
    ShaderUniformLayout(ShaderUniformLayout &&) = delete;

    // descriptor set layout designed for low mutation frequency (changes every frame) uniform data.
    VkDescriptorSetLayout per_frame_desc_set;

    // A descriptor set layout designed for mid mutation frequency (changes every render pass)
    // uniform data.
    VkDescriptorSetLayout per_pass_desc_set;

    // A descriptor set layout designed for high mutation frequency (changes every drawable) uniform
    // data.
    VkDescriptorSetLayout per_drawable_desc_set;

    // A full Vulkan object storing the layout of shader uniform variables.
    VkPipelineLayout layout;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateShaderUniformLayout Gathers information about the layout of uniform variables
 * declared in the shader programs in a graphics pipeline. Two types of uniform layouts are
 * available: push constants and descriptor sets. In particular, only one push constant uniform
 * variable is allowed, though it may be shared in both vertex and fragment shader programs.
 *
 * This function will always return a valid ShaderUniformLayout structure. Any failure occurs during
 * layout creation will make it fail.
 *
 * @param push_constant Specifies the push-constant layout.
 * @param per_frame_desc_set A descriptor set layout designed for low mutation frequency (changes
 * every frame) uniform data.
 * @param per_pass_desc_set A descriptor set layout designed for mid mutation frequency (changes
 * every render pass) uniform data.
 * @param per_drawable_desc_est A descriptor set layout designed for high mutation frequency
 * (changes every drawable) uniform data.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ShaderUniformLayout structure.
 */
std::unique_ptr<ShaderUniformLayout>
CreateShaderUniformLayout(std::optional<VkPushConstantRange> const &push_constant,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_frame_desc_set,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_pass_desc_set,
                          std::vector<VkDescriptorSetLayoutBinding> const &per_drawable_desc_set,
                          VulkanContext *context);

/**
 * @brief The VertexInputInfo struct Stores information about the input attributes of a vertex
 * shader program in a graphics pipeline.
 */
struct VertexInputInfo {
    /**
     * @brief VertexInputInfo Should be created only by calling VertexInputState().
     */
    explicit VertexInputInfo(
        std::vector<VkVertexInputAttributeDescription> const &input_attributes);
    ~VertexInputInfo();

    VertexInputInfo(VertexInputInfo const &) = delete;
    VertexInputInfo(VertexInputInfo &&) = delete;

    // Primary vertex attribute Vulkan description.
    VkPipelineVertexInputStateCreateInfo state;

    // These information needs to be kept alive in order for the state to hold valid memory
    // pointers.
    VkVertexInputBindingDescription input_binding;
    std::vector<VkVertexInputAttributeDescription> input_attributes;
};

/**
 * @brief VertexInputState Creates a structure storing the information about the input attributes of
 * a vertex shader program in a graphics pipeline. In particular, it describes how an attribute
 * associates with data in the vertex buffer.
 *
 * @param input_attributes An array describing all the vertex shader program's input attributes.
 * @return A valid unique pointer to the VertexInputInfo structure.
 */
std::unique_ptr<VertexInputInfo>
CreateVertexInputState(std::vector<VkVertexInputAttributeDescription> const &input_attributes);

/**
 * @brief The FixedStageConfig struct Stores the configuration of the fixed function stages
 * (clipping & rasterization).
 */
struct FixedStageConfig {
    /**
     * @brief FixedStageConfig Should be created only by calling CreateFixedStageConfig().
     */
    FixedStageConfig();
    ~FixedStageConfig();

    FixedStageConfig(FixedStageConfig const &) = delete;
    FixedStageConfig(FixedStageConfig &&) = delete;

    VkPipelineInputAssemblyStateCreateInfo input_assembly;
    VkPipelineRasterizationStateCreateInfo rasterizer;
    VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineDepthStencilStateCreateInfo depth_stencil;
    VkPipelineColorBlendStateCreateInfo color_blending;
    VkPipelineViewportStateCreateInfo viewport_state;

    VkPipelineColorBlendAttachmentState color_blending_attachment;
    VkViewport viewport;
    VkRect2D scissor;
};

/**
 * @brief CreateFixedStageConfig Creates a configuration describing the behavior of fixed function
 * stages (clipping & rasterization).
 *
 * @param polygon_mode Specifies how a polygon should be rasterized.
 * @param cull_mode Specifies if the back/front faces should be excluded.
 * @param enable_depth_test Specifies if a depth rejection should be employed.
 * @param render_target_width The width, in pixels, of the target rendering area.
 * @param render_target_height The height, in pixels, of the target rendering area.
 * @return A valid unique pointer to the FixedStageConfig structure.
 */
std::unique_ptr<FixedStageConfig> CreateFixedStageConfig(VkPolygonMode polygon_mode,
                                                         VkCullModeFlags cull_mode,
                                                         bool enable_depth_test,
                                                         unsigned render_target_width,
                                                         unsigned render_target_height);

/**
 * @brief The FrameBufferAttachment struct Stores the content of a frame buffer attachment,
 * including its memory block. It will clean up the attachment image and image view resources by the
 * end of its lifecycle.
 */
struct FrameBufferAttachment {
    /**
     * @brief FrameBufferAttachmentInfo Should be created only by calling
     * CreateColorAttachmentsForSwapChain() or CreateDepthAttachment.
     */
    FrameBufferAttachment(bool swap_chain_image, VulkanContext *context);
    ~FrameBufferAttachment();

    FrameBufferAttachment(FrameBufferAttachment const &) = delete;
    FrameBufferAttachment(FrameBufferAttachment &&) = delete;

    // Indicates if the underlying image is created along with the swap chain.
    bool swap_chain_image;

    // The memory block which holds the actual image data.
    VkImage image;
    VmaAllocation allocation;

    // Allows the attachment to be referred to in a frame buffer.
    VkImageView view;

    // Allows the attachment to be referred to in a render pass.
    VkAttachmentDescription desc;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateColorAttachmentsForSwapChain Creates a series of frame buffer color attachments for
 * swap chain images. This function will always return a valid FrameBufferAttachment structure.
 *
 * @param context Contextual Vulkan handles.
 * @return An array of frame buffer attachments each of which points to an image in the swap chain,
 * indexed by swap chain image index.
 */
std::vector<std::unique_ptr<FrameBufferAttachment>>
CreateColorAttachmentsForSwapChain(VulkanContext *context);

/**
 * @brief CreateDepthAttachment Creates a depth attachment and allocates an image for depth
 * buffering. This function will always return a valid FrameBufferAttachment structure. Any failure
 * occurs during depth image/image view allocation will make it fail.
 *
 * @param width The width, in pixels, of the depth buffer.
 * @param height The height, in pixels, of the depth buffer.
 * @param samplable Sets this to true if the depth image will be sampled by a shader.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBufferAttachment structure.
 */
std::unique_ptr<FrameBufferAttachment>
CreateDepthAttachment(unsigned width, unsigned height, bool samplable, VulkanContext *context);

/**
 * @brief The RenderPassInfo struct Stores a Vulkan object describing the render pass in a graphics
 * pipeline. It will clean up the render pass resource by the end of its lifecycle.
 */
struct RenderPass {
    /**
     * @brief RenderPassInfo Should be created only by calling CreateRenderPass().
     */
    explicit RenderPass(VulkanContext *context);
    ~RenderPass();

    RenderPass(RenderPass const &) = delete;
    RenderPass(RenderPass &&) = delete;

    // A full Vulkan object storing information of a render pass.
    VkRenderPass pass;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateRenderPass Creates a render pass that has exactly one sub-pass. This function will
 * always return a valid RenderPass structure. Any failure occurs during the render pass creation
 * will make it fail.
 *
 * @param color_attachment_descs Color attachments that will receive outputs of a render pass.
 * @param depth_attachment_desc The depth attachment for depth buffering. This argument is required
 * when depth test is enabled in the FixedStageConfig.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBufferAttachment structure.
 */
std::unique_ptr<RenderPass>
CreateRenderPass(std::vector<VkAttachmentDescription> const &color_attachment_descs,
                 std::optional<VkAttachmentDescription> const &depth_attachment_desc,
                 VulkanContext *context);

/**
 * @brief The GraphicsPipeline struct Stores a Vulkan object describing the graphics pipeline. It
 * will clean up the pipeline resource by the end of its lifecycle.
 */
struct GraphicsPipeline {
    /**
     * @brief GraphicsPipeline Should be created only by calling CreatePipeline().
     */
    explicit GraphicsPipeline(VulkanContext *context);
    ~GraphicsPipeline();

    GraphicsPipeline(GraphicsPipeline const &) = delete;
    GraphicsPipeline(GraphicsPipeline &&) = delete;

    // A full Vulkan object storing information of a graphics pipeline.
    VkPipeline pipeline;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreatePipeline Creates a graphics pipeline. This function will always return a valid
 * GraphicsPipeline structure. Any failure occurs during the graphics pipeline creation will make it
 * fail.
 *
 * @param render_pass The render pass which will be run after binding the pipeline.
 * @param shader_stages The shader programs and stage description.
 * @param inputs Description about the inputs to the vertex shader program.
 * @param layout Layout of the uniform variables declared in the shader programs.
 * @param fixed_stage_config Configurations of the fixed function stages.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the GraphicsPipeline structure.
 */
std::unique_ptr<GraphicsPipeline>
CreateGraphicsPipeline(RenderPass const &render_pass, ShaderStages const &shader_stages,
                       ShaderUniformLayout const &uniform_layout, VertexInputInfo const &inputs,
                       FixedStageConfig const &fixed_stage_config, VulkanContext *context);

/**
 * @brief The FrameBuffer struct A frame buffer stores the output of a render pass. It
 * will clean up the frame buffer resource by the end of its lifecycle.
 */
struct FrameBuffer {
    /**
     * @brief FrameBuffer Should be created only by calling CreateFrameBuffer().
     */
    FrameBuffer(unsigned width, unsigned height, VulkanContext *context);
    ~FrameBuffer();

    FrameBuffer(FrameBuffer const &) = delete;
    FrameBuffer(FrameBuffer &&) = delete;

    // A full Vulkan object storing information of a frame buffer.
    VkFramebuffer buffer;

    // The dimension of the frame buffer.
    unsigned width;
    unsigned height;

    // The values frame buffer attachments will be initialized to at the beginning of a render pass.
    std::vector<VkClearValue> clear_values;

    // Contextual Vulkan handles.
    VulkanContext *context_;
};

/**
 * @brief CreateFrameBuffer Creates a frame buffer for storing the output of a render pass. This
 * function will always return a valid FrameBuffer structure. Any failure occurs during the frame
 * buffer creation will make it fail.
 *
 * @param render_pass The render pass where the frame buffer associates to.
 * @param width The width, in pixels, of the target rendering area.
 * @param height The height, in pixels, of the target rendering area.
 * @param color_attachments The set of image view of the color attachments the frame buffer
 * contains.
 * @param depth_attachment The image view of the depth attachment, if there is any, the frame buffer
 * contains.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the FrameBuffer structure.
 */
std::unique_ptr<FrameBuffer> CreateFrameBuffer(RenderPass const &render_pass, unsigned width,
                                               unsigned height,
                                               std::vector<VkImageView> const &color_attachments,
                                               std::optional<VkImageView> const &depth_attachment,
                                               VulkanContext *context);

/**
 * @brief The UniformBuffer struct It stores data of a uniform variable/block. It will clean up the
 * uniform buffer resource by the end of its lifecycle.
 */
struct UniformBuffer {
    /**
     * @brief UniformBuffer Should be created only by calling CreateUniformBuffer().
     */
    UniformBuffer(unsigned size, VulkanContext *context);
    ~UniformBuffer();

    // A full Vulkan object storing data of the uniform variable/block.
    VkBuffer buffer;

    // The buffer's allocation.
    VmaAllocation allocation;

    // The size of the uniform variable/block
    unsigned size;

    // Contextual Vulkan handles
    VulkanContext *context;
};

/**
 * @brief CreateUniformBuffer Creates a uniform buffer.
 *
 * @param size The size of the uniform buffer in byte.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the UniformBuffer structure.
 */
std::unique_ptr<UniformBuffer> CreateUniformBuffer(unsigned size, VulkanContext *context);

/**
 * @brief The ImageSampler struct For image sampling. It will clean up the sampler resource by the
 * end of its lifecycle.
 */
struct ImageSampler {
    /**
     * @brief ImageSampler Should be created only by calling CreateReadBackSampler().
     */
    ImageSampler(VulkanContext *context);
    ~ImageSampler();

    // A full Vulkan object storing information of a sampler.
    VkSampler sampler;

    // Contextual Vulkan handles.
    VulkanContext *context;
};

/**
 * @brief CreateReadBackSampler Creates an image sampler for the purpose of simply reading back
 * image pixels (no sampling at all).
 *
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ImageSampler structure.
 */
std::unique_ptr<ImageSampler> CreateReadBackSampler(VulkanContext *context);

/**
 * @brief CreateTextureSampler Creates a high quality image sampler for sampling texture images.
 *
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the ImageSampler structure.
 */
std::unique_ptr<ImageSampler> CreateTextureSampler(VulkanContext *context);

/**
 * @brief The DescriptorSets struct It contains descriptor set instances of the descriptor set
 * layouts specified in a ShaderUniformLayout. It will clean up the descriptor set resource by the
 * end of its lifecycle.
 */
struct DescriptorSets {
    /**
     * @brief DescriptorSets Should be created only by calling CreateDescriptorSets().
     */
    DescriptorSets(VulkanContext *context);
    ~DescriptorSets();

    // Per-drawable descriptor set instance.
    VkDescriptorSet drawable;

    // Per-pass descriptor set instance.
    VkDescriptorSet pass;

    // Per-frame descriptor set instance.
    VkDescriptorSet frame;

    // Contextual Vulkan handles.
    VulkanContext *context;
};

/**
 * @brief CreateDescriptorSets Creates descriptor set instances of the descriptor set layouts
 * specified in the shader uniform layout.
 *
 * @param uniform_layout Contains the descriptor set layouts.
 * @param context Contextual Vulkan handles.
 * @return A valid unique pointer to the DescriptorSets structure.
 */
std::unique_ptr<DescriptorSets> CreateDescriptorSets(ShaderUniformLayout const &uniform_layout,
                                                     VulkanContext *context);

/**
 * @brief WriteUniformBufferDescriptor Writes data (from the host) to the specified uniform buffer
 * descriptor. The write is synchronous.
 *
 * @param data Pointer to the host data. The size of the data must be the same as the size of the
 * uniform buffer.
 * @param uniform_buffer The backing uniform buffer to associate with the descriptor.
 * @param descriptor_set The set the descriptor lives in.
 * @param binding Binding of the descriptor in the set.
 * @param context Contextual Vulkan handles.
 */
void WriteUniformBufferDescriptor(void *data, UniformBuffer const &uniform_buffer,
                                  VkDescriptorSet descriptor_set, unsigned binding,
                                  VulkanContext *context);

/**
 * @brief WriteImageDescriptor Writes image data (from the device) to the specified combined
 * image-sampler descriptor. The write is synchronous.
 *
 * @param image_view View of the image data. Note, since image descriptors are always used for image
 * accessing from a shader, the layout of the image is assumed to be
 * VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.
 * @param image_sampler The sampler used for accessing the image content.
 * @param descriptor_set The set the descriptor lives in.
 * @param binding Binding of the descriptor in the set.
 * @param context Contextual Vulkan handles.
 */
void WriteImageDescriptor(VkImageView image_view, ImageSampler const &image_sampler,
                          VkDescriptorSet descriptor_set, unsigned binding, VulkanContext *context);

} // namespace e8

#endif // ISLANDS_RENDERER_PIPELINE_COMMON_H
