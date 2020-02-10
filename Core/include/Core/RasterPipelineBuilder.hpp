#pragma once

#include "Core/PipelineBuilder.hpp"
#include "Core/PipelineLayout.hpp"
#include "Core/RenderPass.hpp"

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
        // void setDepthStensilState();

        /**
         * Add the pipeline layout for this pipeline.
         * Note one must be added for creation to succeed.
         * @param layout: A previously created pipeline layout
         */
        void setPipelineLayout(const PipelineLayout& layout);

        /**
         * Add the RenderPass for this pipeline.
         * Note one must be added for creation to succeed.
         * @param renderPass: A previously created RenderPass
         * @param subpass: The subpass of this pipeline within renderPass
         */
        void setRenderPass(const RenderPass& renderPass, uint32_t subpass);

        /**
         * Add the blend state for the next colour attachment.
         * This must be called in order for each colour attachment that will be
         * included in the framebuffer
         * @param blendState: The blend state for the next colour attachment
         */
        void addColourAttachmentBlendState(const vk::PipelineColorBlendAttachmentState& state);

        /**
         * Add a default blend state that effectively replaces the previous.
         */
        void addColourAttachmentBlendState();

        /// -- End members for configuration --

    protected:
        /// -- Sensible defaults for more generically configurable parts --
        /// These values are stored in this object for memory management

        vk::Viewport m_viewport{
            0.0f,
            0.0f,
            0.0f,
            0.0f, // Should be set before use with setWindowSize()
            0.0f,
            1.0f,
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

        vk::PipelineColorBlendStateCreateInfo m_colourBlendState{
            vk::PipelineColorBlendStateCreateFlags(),
            VK_FALSE,
            vk::LogicOp::eCopy,
            0,
            nullptr,
            {0, 0, 0, 0},
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

        /// A list of colour attachment blend states, specific to this subpass
        std::vector<vk::PipelineColorBlendAttachmentState> m_colourBlendStates;

        /// Note these default values are invalid for pipeline creation.
        vk::PipelineLayout m_pipelineLayout = vk::PipelineLayout();
        vk::RenderPass m_renderPass = vk::RenderPass();
        uint32_t m_subpass = 0;
    };
}
