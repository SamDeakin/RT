#pragma once

#include <vulkan/vulkan.hpp>

class Framebuffer;

namespace Core {
    class FramebufferBuilder {
    public:
        FramebufferBuilder();
        ~FramebufferBuilder();

        void addImageView(const vk::ImageView& imageView)

    private:
        /// Once configured, the Framebuffer ctor will use this class for construction
        friend Framebuffer;
    };
}
