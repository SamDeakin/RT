#include "Core/GraphicsPipeline.hpp"

namespace Core {

    GraphicsPipeline::GraphicsPipeline(vk::Device& device, vk::Pipeline& pipeline)
        : m_device(device)
        , m_pipeline(pipeline){}

    GraphicsPipeline::~GraphicsPipeline() {
        m_device.destroyPipeline(m_pipeline);
    }
}
