#pragma once

#include "Core/DescriptorSetLayout.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace Core {
    class PipelineLayout {
    public:
        PipelineLayout(vk::Device& device,
                       uint32_t descriptorSetLayoutCount,
                       const vk::DescriptorSetLayout* descriptorSetLayouts,
                       uint32_t pushConstantRangeCount,
                       const vk::PushConstantRange* pushConstantRanges);
        ~PipelineLayout();

        [[nodiscard]] const vk::PipelineLayout& getHandle() const;

    private:
        vk::Device& m_device;

        vk::PipelineLayout m_handle;
    };
}
