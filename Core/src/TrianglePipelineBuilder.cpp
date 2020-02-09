#include "Core/TrianglePipelineBuilder.hpp"

namespace Core {
    TrianglePipelineBuilder::TrianglePipelineBuilder() {
        m_shaderStageCreateInfos.reserve(s_shaderStageCount);
        m_shaderStageCreateInfos.emplace_back();
        m_shaderStageCreateInfos.emplace_back();
    }

    TrianglePipelineBuilder::~TrianglePipelineBuilder() = default;

    void TrianglePipelineBuilder::getPipelineCreateInfo(vk::GraphicsPipelineCreateInfo& createInfo) {
        RasterPipelineBuilder::getPipelineCreateInfo(createInfo);

        createInfo.pInputAssemblyState = &m_inputAssemblyState;

        createInfo.stageCount = m_shaderStageCreateInfos.size();
        createInfo.pStages = m_shaderStageCreateInfos.data();

        createInfo.pVertexInputState = &m_vertexInputStateCreateInfo;
    }

    void TrianglePipelineBuilder::addVertexShader(Core::Shader& shader) { m_shaderStageCreateInfos[0] = shader.getPipelineStageCreateInfo(); }

    void TrianglePipelineBuilder::addFragmentShader(Shader& shader) { m_shaderStageCreateInfos[1] = shader.getPipelineStageCreateInfo(); }

    void TrianglePipelineBuilder::addVertexInputBindingDesc(const vk::VertexInputBindingDescription& bindingDescription) {
        m_vertexBindingDescriptions.push_back(bindingDescription);
        m_vertexInputStateCreateInfo.vertexBindingDescriptionCount = m_vertexBindingDescriptions.size();
        m_vertexInputStateCreateInfo.pVertexBindingDescriptions = m_vertexBindingDescriptions.data();
    }

    void TrianglePipelineBuilder::addVertexInputAttributeDesc(const vk::VertexInputAttributeDescription& attributeDescription) {
        m_vertexAttributeDescriptions.push_back(attributeDescription);
        m_vertexInputStateCreateInfo.vertexAttributeDescriptionCount = m_vertexAttributeDescriptions.size();
        m_vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_vertexAttributeDescriptions.data();
    }
}
