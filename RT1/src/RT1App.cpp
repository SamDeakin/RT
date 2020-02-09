#include "RT1/RT1App.hpp"

#include <Core/Shader.hpp>
#include <Core/TrianglePipelineBuilder.hpp>

namespace {
    struct Vertex {
        float x, y, z, w;
    };
}

namespace RT1 {

    RT1App::RT1App(Core::Renderer& renderer)
        : m_renderer(renderer)
        , m_device(renderer.getDevice()){
        Core::Shader vertShader("Resources/Shaders/trivial.vert.spv", Core::ShaderType::eVertex, renderer.getDevice());
        Core::Shader fragShader("Resources/Shaders/xyzToRgb.frag.spv", Core::ShaderType::eFragment, renderer.getDevice());

        Core::TrianglePipelineBuilder pipelineBuilder;
        pipelineBuilder.addShader(vertShader);
        pipelineBuilder.addShader(fragShader);

        vk::Extent2D windowSize = renderer.getSwapchainExtents();
        pipelineBuilder.setWindowSize(windowSize.width, windowSize.height);

        pipelineBuilder.addVertexInputBindingDesc(vk::VertexInputBindingDescription{
            0,
            sizeof(Vertex),
            vk::VertexInputRate::eVertex,
        });
        pipelineBuilder.addVertexInputAttributeDesc(vk::VertexInputAttributeDescription{
            0,
            0,
            vk::Format::eR32G32B32A32Sfloat,
            0,
        });

        vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
        pipelineBuilder.getPipelineCreateInfo(pipelineCreateInfo);

        std::vector<std::unique_ptr<Core::GraphicsPipeline>> createdPipelines = m_renderer.createGraphicsPipelines(1, &pipelineCreateInfo);

        m_simpleTrianglePipeline = std::move(createdPipelines[0]);
    }

    RT1App::~RT1App() = default;

    void RT1App::renderFrame(Core::TimePoint now, Core::TimeDelta delta) {}
}
