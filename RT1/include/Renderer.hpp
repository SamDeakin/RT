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
                 const char** deviceExtensions,
                 vk::PhysicalDeviceFeatures features);

        Renderer(const Renderer&) = delete;
        void operator=(const Renderer&) = delete;

        ~Renderer() noexcept;

    private:
        vk::Instance m_instance;
        vk::Device m_device;

        // Tracking enabled features
        std::vector<vk::ExtensionProperties> m_instanceExtensions;
        std::vector<vk::LayerProperties> m_instanceLayers;
        std::vector<vk::ExtensionProperties> m_deviceExtensions;
        std::vector<vk::DeviceQueueInfo2> m_deviceQueues;
        vk::PhysicalDeviceFeatures m_features;

        // -- ctor helper functions --

        /// Initialize m_instance
        void initInstance(uint32_t glfwExtensionCount,
                          const char** glfwExtensions,
                          uint32_t instanceExtensionCount,
                          const char** instanceExtensions,
                          uint32_t instanceLayerCount,
                          const char** instanceLayers);
        /// Used by initInstance
        void addInstanceExtensions(uint32_t glfwExtensionCount, const char** glfwExtensions, uint32_t instanceExtensionCount, const char** instanceExtensions);
        void addInstanceLayers(uint32_t instanceLayerCount, const char** instanceLayers);

        /// Initialize m_device
        void initDevice(uint32_t deviceExtensionCount, const char** deviceExtensions, vk::PhysicalDeviceFeatures features);
        /// Used by initDevice: these will be called for a candidate device
        /// They are nodiscard because the device may not be suitable, in which case the function did not complete successfully
        /// They must be retried with a new device or the program should exit.
        [[nodiscard]] bool addDeviceFeatures(vk::PhysicalDeviceFeatures features);
        [[nodiscard]] bool addDeviceExtensions(uint32_t deviceExtensionCount, const char** deviceExtensions);
        [[nodiscard]] bool addDeviceQueues();

        // -- end ctor helper functions --
    };

}