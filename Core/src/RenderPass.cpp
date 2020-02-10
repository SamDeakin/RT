#include "Core/RenderPass.hpp"

namespace Core {
    RenderPass::RenderPass(vk::Device& device, const vk::RenderPassCreateInfo& createInfo)
        : m_device(device)
        , m_handle(m_device.createRenderPass(createInfo)) {}

    RenderPass::~RenderPass() { m_device.destroyRenderPass(m_handle); }

    const vk::RenderPass& RenderPass::getHandle() const { return m_handle; }
}
