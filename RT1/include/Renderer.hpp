#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace Core {

    /**
     * A class for managing and accessing vulkan configuration. You probably don't want to construct
     * this object directly, you should make a WindowedRenderer<Window> instead.
     */
    class Renderer {
    public:
        Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer(const Renderer&&) = delete;
        void operator=(const Renderer&) = delete;
        void operator=(const Renderer&&) = delete;

        virtual ~Renderer() noexcept;

    protected:
        /// Vulkan instance configuration
        vk::Instance m_instance;
        std::vector<vk::ExtensionProperties> m_instanceExtensions;
        std::vector<vk::LayerProperties> m_instanceLayers;

        /// Vulkan physical device configuration
        vk::PhysicalDevice m_physicalDevice;
        vk::PhysicalDeviceProperties m_deviceProperties;
        vk::PhysicalDeviceFeatures m_features;
        std::vector<vk::ExtensionProperties> m_deviceExtensions;
        std::vector<vk::QueueFamilyProperties> m_deviceQueueFamilies;

        /// Vulkan logical device configuration
        vk::Device m_device;
        std::vector<vk::Queue> m_graphicsQueues;
        std::vector<vk::Queue> m_transferQueues;
        std::vector<vk::Queue> m_computeQueues;

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

        /// Choose a physical device and initialize parameters for the creation of a logical device
        void initPhysicalDevice(uint32_t deviceExtensionCount, const char** deviceExtensions, vk::PhysicalDeviceFeatures features);
        /// Used by initPhysicalDevice: these will be called for a candidate device
        /// They are nodiscard because the device may not be suitable, in which case the function did not complete successfully
        /// They must be retried with a new device or the program should exit.
        [[nodiscard]] bool addDeviceProperties();
        [[nodiscard]] bool addDeviceFeatures(vk::PhysicalDeviceFeatures features);
        [[nodiscard]] bool addDeviceExtensions(uint32_t deviceExtensionCount, const char** deviceExtensions);
        [[nodiscard]] bool addDeviceQueues();

        /// Initialize m_device
        void initLogicalDevice();

        // -- end ctor helper functions --
    };

}