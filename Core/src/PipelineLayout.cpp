#include "Core/PipelineLayout.hpp"

namespace Core {
    PipelineLayout::PipelineLayout(vk::Device& device,
                                   uint32_t descriptorSetLayoutCount,
                                   const vk::DescriptorSetLayout* descriptorSetLayouts,
                                   uint32_t pushConstantRangeCount,
                                   const vk::PushConstantRange* pushConstantRanges)
        : m_device(device) {
        vk::PipelineLayoutCreateInfo createInfo{
            vk::PipelineLayoutCreateFlags(),
            descriptorSetLayoutCount,
            descriptorSetLayouts,
            pushConstantRangeCount,
            pushConstantRanges,
        };

        m_handle = m_device.createPipelineLayout(createInfo);
    }

    PipelineLayout::~PipelineLayout() { m_device.destroyPipelineLayout(m_handle); }

    const vk::PipelineLayout& PipelineLayout::getHandle() const { return m_handle; }
}
