#include "Core/TrianglePipelineBuilder.hpp"

namespace Core {
    TrianglePipelineBuilder::TrianglePipelineBuilder() = default;
    TrianglePipelineBuilder::~TrianglePipelineBuilder() = default;

    void TrianglePipelineBuilder::getPipelineCreateInfo(vk::GraphicsPipelineCreateInfo& createInfo) {
        RasterPipelineBuilder::getPipelineCreateInfo(createInfo);

        createInfo.pInputAssemblyState = &m_inputAssemblyState;

        // TODO Vertex input
        // TODO Shader stages
    }

    void TrianglePipelineBuilder::addVertexShader(Core::Shader& shader) { m_vertexShader = &shader; }
}
