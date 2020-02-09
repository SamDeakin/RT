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

        // TODO Vertex input
    }

    void TrianglePipelineBuilder::addVertexShader(Core::Shader& shader) {
        m_shaderStageCreateInfos[0] = shader.getPipelineStageCreateInfo();
    }
    void TrianglePipelineBuilder::addFragmentShader(Shader& shader) {
        m_shaderStageCreateInfos[1] = shader.getPipelineStageCreateInfo();
    }
}
