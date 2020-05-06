#pragma once

#include <vulkan/vulkan.hpp>

namespace Core {
    class GraphicsPipeline {
    public:
        GraphicsPipeline(vk::Device& device, vk::Pipeline& pipeline);
        ~GraphicsPipeline();

        operator vk::Pipeline() const;

    private:
        vk::Device& m_device;

        /// The pipeline handle, as owned by this GraphicsPipeline object
        vk::Pipeline m_pipeline;
    };
}
