#pragma once

#include "Core/PipelineBuilder.hpp"

namespace Core {
    class RasterPipelineBuilder : public PipelineBuilder {
    public:
        RasterPipelineBuilder();
        ~RasterPipelineBuilder() override;

        /**
         * Get the configured createInfo object that should be used to create this pipeline.
         * The intention is that it will be grouped with other pipelines for creation.
         * @param createInfo: The object that will be filled with the info necessary to create the pipeline
         */
        virtual void getPipelineCreateInfo(vk::GraphicsPipelineCreateInfo& createInfo);

        /// -- Members for configuration --

        void setWindowSize(uint32_t width, uint32_t height);

        // TODO
        //void setDepthStensilState();

        // TODO
        //void setPipelineLayout();

        // TODO
        //void setRenderPass();

        /// -- End members for configuration --

    protected:
        void addFragmentShader(Shader& shader) override;

        Shader* m_fragmentShader = nullptr;

        /// -- Sensible defaults for more generically configurable parts --
        /// These values are stored in this object for memory management

        vk::Viewport m_viewport{
            0.0f, 0.0f,
            0.0f, 0.0f, // Should be set before use with setWindowSize()
            0.0f, 1.0f,
        };
        vk::Rect2D m_scissors{
            vk::Offset2D(0, 0),
            vk::Extent2D(0, 0), // Should be set before use with setWindowSize()
        };
        vk::PipelineViewportStateCreateInfo m_viewportState{
            vk::PipelineViewportStateCreateFlags(),
            1,
            &m_viewport,
            1,
            &m_scissors,
        };

        vk::PipelineRasterizationStateCreateInfo m_rasterizationState{
            vk::PipelineRasterizationStateCreateFlags(),
            VK_FALSE, // Depth clamp
            VK_FALSE, // Discard
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eClockwise,

            // Depth bias features
            VK_FALSE,
            0.0f,
            0.0f,
            0.0f,

            1.0f, // Line width
        };

        vk::PipelineMultisampleStateCreateInfo m_multisampleState{
            vk::PipelineMultisampleStateCreateFlags(),
            vk::SampleCountFlagBits::e1,

            // Sample Shading features
            VK_FALSE,
            1.0f,

            // Misc options
            nullptr,
            VK_FALSE,
            VK_FALSE,
        };

        constexpr static const vk::DynamicState s_defaultDynamicStates[] = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eLineWidth,
        };
        constexpr static const uint32_t s_defaultDynamicStateCount = sizeof(s_defaultDynamicStateCount) / sizeof(s_defaultDynamicStates[0]);
        vk::PipelineDynamicStateCreateInfo m_dynamicState{
            vk::PipelineDynamicStateCreateFlags(),
            s_defaultDynamicStateCount,
            s_defaultDynamicStates,
        };

        /// -- End stored create info default structures --
    };
}
