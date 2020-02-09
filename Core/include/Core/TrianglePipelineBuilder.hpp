#pragma once

#include "Core/RasterPipelineBuilder.hpp"

#include <vector>

namespace Core {
    class TrianglePipelineBuilder : public RasterPipelineBuilder {
    public:
        TrianglePipelineBuilder();
        ~TrianglePipelineBuilder() override;

        void getPipelineCreateInfo(vk::GraphicsPipelineCreateInfo& createInfo) override;

        /// -- Members for configuration --

        /**
         * Add a vertex binding description
         * Note: This object should be destroyed before the added description
         * The calling order matters for this method.
         * @param bindingDescription: The description to add
         */
        void addVertexInputBindingDesc(const vk::VertexInputBindingDescription& bindingDescription);

        /**
         * Add a vertex attribute description
         * Note: This object should be destroyed before the added description
         * The calling order matters for this method.
         * @param attributeDescription: The description to add
         */
        void addVertexInputAttributeDesc(const vk::VertexInputAttributeDescription& attributeDescription);

        /// -- End members for configuration --

    protected:
        void addVertexShader(Shader& shader) override;
        void addFragmentShader(Shader& shader) override;

        vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyState{
            vk::PipelineInputAssemblyStateCreateFlags(),
            vk::PrimitiveTopology::eTriangleList,
            VK_FALSE,
        };

        std::vector<vk::VertexInputBindingDescription> m_vertexBindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> m_vertexAttributeDescriptions;
        vk::PipelineVertexInputStateCreateInfo m_vertexInputStateCreateInfo; // The default is zeroed out

        constexpr static const std::size_t s_shaderStageCount = 2;
        std::vector<vk::PipelineShaderStageCreateInfo> m_shaderStageCreateInfos;
    };
}
