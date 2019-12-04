#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace Core {

    class Renderer {
    public:
        /**
         * Construct a Renderer with the desired extensions and layers.
         * Throws a std::runtimeException if it cannot be created.
         * @param instanceExtensions: Desired instance extensions
         * @param instanceLayers: Desired instance layers
         * @param deviceExtensions: Desired device extensions
         */
        Renderer(uint32_t glfwExtensionCount,
                 const char** glfwExtensions,
                 uint32_t instanceExtensionCount,
                 const char** instanceExtensions,
                 uint32_t instanceLayerCount,
                 const char** instanceLayers,
                 uint32_t deviceExtensionCount,
                 const char** deviceExtensions);

        Renderer(const Renderer&) = delete;
        void operator=(const Renderer&) = delete;

        ~Renderer() noexcept;

    private:
        vk::Instance m_instance;
        vk::Device m_device;

        // Tracking enabled layers and extensions
        std::vector<vk::ExtensionProperties> m_instanceExtensions;
        std::vector<vk::ExtensionProperties> m_deviceExtensions;
        std::vector<vk::LayerProperties> m_instanceLayers;

        // -- ctor helper functions --

        void initInstance(uint32_t glfwExtensionCount,
                          const char** glfwExtensions,
                          uint32_t instanceExtensionCount,
                          const char** instanceExtensions,
                          uint32_t instanceLayerCount,
                          const char** instanceLayers);
        void addInstanceExtensions(uint32_t glfwExtensionCount, const char** glfwExtensions, uint32_t instanceExtensionCount, const char** instanceExtensions);
        void addInstanceLayers(uint32_t instanceLayerCount, const char** instanceLayers);
        void initDevice(uint32_t deviceExtensionCount, const char** deviceExtensions);
        void addDeviceExtensions(uint32_t deviceExtensionCount, const char** deviceExtensions);

        // -- end ctor helper functions --
    };

}