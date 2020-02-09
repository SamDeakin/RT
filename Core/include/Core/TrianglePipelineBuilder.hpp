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

        /// -- End members for configuration --

    protected:
        void addVertexShader(Shader& shader) override;
        void addFragmentShader(Shader& shader) override;

        vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyState{
            vk::PipelineInputAssemblyStateCreateFlags(),
            vk::PrimitiveTopology::eTriangleList,
            VK_FALSE,
        };

        constexpr static const std::size_t s_shaderStageCount = 2;
        std::vector<vk::PipelineShaderStageCreateInfo> m_shaderStageCreateInfos;
    };
}
