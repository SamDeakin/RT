#include "Core/RenderPassBuilder.hpp"

namespace Core {
    RenderPassBuilder::RenderPassBuilder() = default;
    RenderPassBuilder::~RenderPassBuilder() = default;

    void RenderPassBuilder::getRenderPassCreateInfo(vk::RenderPassCreateInfo& createInfo) {
        createInfo.flags = vk::RenderPassCreateFlags();
        createInfo.attachmentCount = m_attachments.size();
        createInfo.pAttachments = m_attachments.data();
        createInfo.subpassCount = m_subpasses.size();
        createInfo.pSubpasses = m_subpasses.data();
        createInfo.dependencyCount = m_dependencies.size();
        createInfo.pDependencies = m_dependencies.data();
    }

    void RenderPassBuilder::addAttachment(const vk::AttachmentDescription& description) { m_attachments.push_back(description); }
    void RenderPassBuilder::addSubpass(const vk::SubpassDescription& description) { m_subpasses.push_back(description); }
    void RenderPassBuilder::addDependency(const vk::SubpassDependency& dependency) { m_dependencies.push_back(dependency); }
}
