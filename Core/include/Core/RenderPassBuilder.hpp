#pragma once

#include <vulkan/vulkan.hpp>

#include <vector>

namespace Core {
    class RenderPassBuilder {
    public:
        RenderPassBuilder();
        ~RenderPassBuilder();

        /**
         * Get the necessary information to construct a vk::RenderPass.
         * Note it should be constructed before destroying this object for
         * memory management reasons.
         * @param createInfo: The struct that will be filled with the results
         */
        void getRenderPassCreateInfo(vk::RenderPassCreateInfo& createInfo);

        void addAttachment(const vk::AttachmentDescription& description);
        void addSubpass(const vk::SubpassDescription& description);
        void addDependency(const vk::SubpassDependency& dependency); // TODO Calculate this dependency?

    private:
        std::vector<vk::AttachmentDescription> m_attachments;
        std::vector<vk::SubpassDescription> m_subpasses;
        std::vector<vk::SubpassDependency> m_dependencies;
    };
}
