#include "Core/Shader.hpp"

#include <iostream>
#include <vector>

namespace Core {
    Shader::Shader(const std::string& filename, Core::ShaderType type, vk::Device& device)
        : m_name(filename)
        , m_type(type)
        , m_device(device) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Can't load shader from file: " + filename);
        }

        // TODO Funky endianness problems here
        std::vector<std::byte> shaderBytes(file.tellg());

        file.seekg(0);
        file.read(shaderBytes.data(), shaderBytes.size());
        file.close();

        vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
            vk::ShaderModuleCreateFlags(),
            shaderBytes.size(),
            static_cast<uint32_t*>(shaderBytes.data()),
        };

        m_shaderModule = device.createShaderModule(shaderModuleCreateInfo);
    }

    Shader::~Shader() { m_device.destroyShaderModule(m_shaderModule); }

    vk::PipelineShaderStageCreateInfo Shader::getPipelineStageCreateInfo() {
        return vk::PipelineShaderStageCreateInfo{
            vk::PipelineShaderStageCreateFlags(),
            m_type, // Just a direct translation for now
            m_shaderModule,
            "main", // Simple classic
            nullptr,
        };
    }
}
