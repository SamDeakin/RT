#pragma once

#include "Core/RenderTypes.hpp"

#include <vulkan/vulkan.hpp>

namespace Core {

    class Shader {
    public:
        Shader(const std::string& filename, ShaderType type, vk::Device& device);
        ~Shader();

        /// Retrieve the info needed to use this shader in a pipeline
        vk::PipelineShaderStageCreateInfo getPipelineStageCreateInfo();
    private:
        /// A hopefully descriptive name for the shader
        const std::string m_name;

        /// The type of shader held by this class
        ShaderType m_type;

        /// A reference to the device, used for cleanup
        vk::Device& m_device;

        /// The loaded shader
        vk::ShaderModule m_shaderModule;
    };
}
