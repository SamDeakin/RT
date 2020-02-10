#include "Core/RasterPipelineBuilder.hpp"

namespace Core {
    RasterPipelineBuilder::RasterPipelineBuilder() = default;
    RasterPipelineBuilder::~RasterPipelineBuilder() = default;

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
        createInfo.pColorBlendState = &m_colourBlendState;

            // Previously created object handles that cannot be merged with another
        createInfo.layout = m_pipelineLayout;

        createInfo.renderPass = m_renderPass;
        createInfo.subpass = m_subpass;

        // Members from PipelineBuilder
        createInfo.basePipelineHandle = m_basePipeline;
        createInfo.basePipelineIndex = m_basePipelineIndex;
    }

    void RasterPipelineBuilder::setWindowSize(uint32_t width, uint32_t height) {
        m_viewport.width = width;
        m_viewport.height = height;
        m_scissors.extent = vk::Extent2D(width, height);
    }

    void RasterPipelineBuilder::setPipelineLayout(const PipelineLayout& layout) {
        m_pipelineLayout = layout.getHandle();
    }

    void RasterPipelineBuilder::setRenderPass(const RenderPass& renderPass, uint32_t subpass) {
        m_renderPass = renderPass.getHandle();
        m_subpass = subpass;
    }

    void RasterPipelineBuilder::addColourAttachmentBlendState(const vk::PipelineColorBlendAttachmentState &state) {
        m_colourBlendStates.push_back(state);
        m_colourBlendState.attachmentCount = m_colourBlendStates.size();
        m_colourBlendState.pAttachments = m_colourBlendStates.data();
    }

    void RasterPipelineBuilder::addColourAttachmentBlendState() {
        addColourAttachmentBlendState(vk::PipelineColorBlendAttachmentState{
            VK_FALSE,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
        });
    }
}
