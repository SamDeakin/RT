#include "RT1/RT1App.hpp"

#include <Core/Shader.hpp>
#include <Core/TrianglePipelineBuilder.hpp>

namespace RT1 {

    RT1App::RT1App(Core::Renderer& renderer)
        : m_renderer(renderer)
        , m_device(renderer.getDevice()){
        Core::Shader vertShader("Resources/Shaders/trivial.vert.spv", Core::ShaderType::eVertex, renderer.getDevice());
        Core::Shader fragShader("Resources/Shaders/xyzToRgb.frag.spv", Core::ShaderType::eFragment, renderer.getDevice());

        Core::TrianglePipelineBuilder pipelineBuilder;
        pipelineBuilder.addShader(vertShader);
        pipelineBuilder.addShader(fragShader);

        // TODO Config more

        vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
        pipelineBuilder.getPipelineCreateInfo(pipelineCreateInfo);

        std::vector<std::unique_ptr<Core::GraphicsPipeline>> createdPipelines = m_renderer.createGraphicsPipelines(1, &pipelineCreateInfo);

        m_simpleTrianglePipeline = std::move(createdPipelines[0]);
    }

    RT1App::~RT1App() = default;

    void RT1App::renderFrame(Core::TimePoint now, Core::TimeDelta delta) {}
}
