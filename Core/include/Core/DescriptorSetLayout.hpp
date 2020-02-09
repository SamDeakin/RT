#pragma once

#include <vulkan/vulkan.hpp>

namespace Core {
    class DescriptorSetLayout {
    public:
        DescriptorSetLayout(vk::Device& device, uint32_t bindingCount, const vk::DescriptorSetLayoutBinding* bindings);
        ~DescriptorSetLayout();

        [[nodiscard]] const vk::DescriptorSetLayout& getHandle() const;

    private:
        vk::Device& m_device;
        vk::DescriptorSetLayout m_handle;
    };
}
