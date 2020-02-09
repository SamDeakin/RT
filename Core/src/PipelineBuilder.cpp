#include "Core/PipelineBuilder.hpp"

namespace Core {
    PipelineBuilder::PipelineBuilder() = default;
    PipelineBuilder::~PipelineBuilder() = default;

    void PipelineBuilder::addShader(Shader &shader) {
        ShaderType type = shader.getType();
        switch(type) {
        case ShaderType::eVertex:
            addVertexShader(shader);
            break;
        case ShaderType::eFragment:
            addFragmentShader(shader);
            break;
        default:
            throw std::runtime_error(std::string("Shader type not yet supported: ") + vk::to_string(type));
        }
    }

    void PipelineBuilder::addVertexShader(Core::Shader& shader) {
        throw std::runtime_error(std::string("This pipeline does not support shader type: ") + vk::to_string(shader.getType()));
    }

    void PipelineBuilder::addFragmentShader(Core::Shader& shader) {
        throw std::runtime_error(std::string("This pipeline does not support shader type: ") + vk::to_string(shader.getType()));
    }

    void PipelineBuilder::setBasePipeline(vk::Pipeline pipeline) {
        m_basePipeline = pipeline;
        m_basePipelineIndex = -1;
        m_pipelineCreateFlags |= vk::PipelineCreateFlagBits::eDerivative;
    }

    void PipelineBuilder::setBasePipeline(int32_t index) {
        m_basePipeline = vk::Pipeline(); // Null handle
        m_basePipelineIndex = index;
        m_pipelineCreateFlags |= vk::PipelineCreateFlagBits::eDerivative;
    }
}
