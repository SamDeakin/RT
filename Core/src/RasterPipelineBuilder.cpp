#include "Core/RasterPipelineBuilder.hpp"

namespace Core {
    RasterPipelineBuilder::RasterPipelineBuilder() = default;
    RasterPipelineBuilder::~RasterPipelineBuilder() = default;

    void RasterPipelineBuilder::addFragmentShader(Core::Shader& shader) { m_fragmentShader = &shader; }

    void RasterPipelineBuilder::getPipelineCreateInfo(vk::GraphicsPipelineCreateInfo& createInfo) {
        // Member from PipelineBuilder
        createInfo.flags = m_pipelineCreateFlags;

        // Must be overriden by a subclass
        createInfo.stageCount = 0;
        createInfo.pStages = nullptr;
        createInfo.pVertexInputState = nullptr;

        // Can be overriden without reconciliation by subclass
        createInfo.pInputAssemblyState = nullptr;
        createInfo.pTessellationState = nullptr;
        createInfo.pDepthStencilState = nullptr; // TODO

        // A subclass may have to merge these with their own settings
        createInfo.pViewportState = &m_viewportState;
        createInfo.pRasterizationState = &m_rasterizationState;
        createInfo.pMultisampleState = &m_multisampleState;
        createInfo.pDynamicState = &m_dynamicState;

        // TODO Pipeline layout
        // TODO Colour blend states
        // TODO Renderpasses
        // TODO subpass

        // Members from PipelineBuilder
        createInfo.basePipelineHandle = m_basePipeline;
        createInfo.basePipelineIndex = m_basePipelineIndex;
    }

    void RasterPipelineBuilder::setWindowSize(uint32_t width, uint32_t height) {
        m_viewport.width = width;
        m_viewport.height = height;
        m_scissors.extent = vk::Extent2D(width, height);
    }
}
