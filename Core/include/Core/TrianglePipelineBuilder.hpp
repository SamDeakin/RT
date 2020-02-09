#pragma once

#include "Core/RasterPipelineBuilder.hpp"

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

        vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyState{
            vk::PipelineInputAssemblyStateCreateFlags(),
            vk::PrimitiveTopology::eTriangleList,
            VK_FALSE,
        };

    private:
        Shader* m_vertexShader = nullptr;
    };
}
