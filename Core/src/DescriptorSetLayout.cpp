#include "Core/DescriptorSetLayout.hpp"

namespace Core {
    DescriptorSetLayout::DescriptorSetLayout(vk::Device& device, uint32_t bindingCount, const vk::DescriptorSetLayoutBinding* bindings)
        : m_device(device) {
        vk::DescriptorSetLayoutCreateInfo createInfo{
            vk::DescriptorSetLayoutCreateFlags(),
            bindingCount,
            bindings,
        };

        m_handle = m_device.createDescriptorSetLayout(createInfo);
    }

    DescriptorSetLayout::~DescriptorSetLayout() { m_device.destroyDescriptorSetLayout(m_handle); }

    const vk::DescriptorSetLayout& DescriptorSetLayout::getHandle() const { return m_handle; }
}
