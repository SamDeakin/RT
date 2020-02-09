#include "Core/Shader.hpp"

#include <fstream>
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

        // This gets a bit funky because we have to load and specify length in bytes
        // But we pass the final data as an array of uint32_t
        std::size_t shaderByteLength = file.tellg();
        std::vector<char> shaderData(shaderByteLength);

        file.seekg(0);
        file.read(shaderData.data(), shaderByteLength);
        file.close();

        vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
            vk::ShaderModuleCreateFlags(),
            shaderByteLength,
            reinterpret_cast<uint32_t*>(shaderData.data()),
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

    ShaderType Shader::getType() { return m_type; }
}
