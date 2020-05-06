#pragma once

#include <vulkan/vulkan.hpp>

namespace Core {
    class RenderPass {
    public:
        RenderPass(vk::Device& device, const vk::RenderPassCreateInfo& createInfo);
        ~RenderPass();

        [[nodiscard]] const vk::RenderPass& getHandle() const;

        operator vk::RenderPass() const;

    private:
        vk::Device& m_device;
        vk::RenderPass m_handle;
    };
}
