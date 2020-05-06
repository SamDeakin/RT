#pragma once

#include "Core/GraphicsPipeline.hpp"
#include "Core/RenderTypes.hpp"

#include <memory>
#include <set>
#include <unordered_map>
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

        /**
         * Get a device created for rendering
         * @return The device
         */
        vk::Device getDevice() const;

        /**
         * Get a physical device to be used for memory allocation
         * Currently there is only one physical device used.
         * @return A physical device
         */
        vk::PhysicalDevice getPhysicalDevice() const;

        /**
         * Get the vulkan instance
         * @return The vulkan instance
         */
        vk::Instance getInstance() const;

        /**
         * Get the current window size of the swapchain
         * @return The extents of the swapchain
         */
        const vk::Extent2D& getSwapchainExtents() const;

        /**
         * Get the images that were received from the current swapchain
         * @return A non-mutable vector of the images.
         */
        const std::vector<vk::Image>& getSwapchainImages() const;

        /**
         * Get the number of swapchain images currently known to exist.
         * @return In practice, the size of getSwapchainImages()
         */
        std::size_t getNumSwapchainImages() const;

        /**
         * Get the output format used in the swapchain
         * @return The format used to construct the swapchain
         */
        const vk::Format& getOutputFormat() const;

        /**
         * Get a command pool for the given queue type
         * @param type: The primary queue type requested
         * @return A valid command pool, not necessarily distinct from one previously returned by this function.
         */
        vk::CommandPool getCommandPool(QueueType type);

        /**
         * Tells the renderer to destroy any swapchains it has, and then create any new ones needed.
         * Useful for situations where previous swapchains have been invalidated.
         * Must be called before rendering can begin for the first time.
         * @param extents: The size of the window that the swapchain is rendering to.
         */
        void recreateSwapChain(vk::Extent2D windowExtents);

        /**
         * A helper to create many pipeline objects in a single call
         * @param count: The number of pipelines to create
         * @param createInfos: A pointer to the contiguous array of createInfos
         * @return The created pipelines, where the caller is responsible for memory management
         */
        std::vector<std::unique_ptr<GraphicsPipeline>> createGraphicsPipelines(uint32_t count, const vk::GraphicsPipelineCreateInfo* createInfos);

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
        std::unordered_map<QueueType, QueueGroup> m_queues;
        /// Command pool sets, closely linked to queues
        std::unordered_map<QueueType, std::pair<std::size_t, std::vector<vk::CommandPool>>> m_commandPools;
        std::set<QueueType> m_createdCommandPoolTypes;

        /// Vulkan surface configuration
        vk::SurfaceKHR m_surface;
        vk::SurfaceFormatKHR m_surfaceFormat;
        vk::PresentModeKHR m_presentMode;
        vk::SwapchainKHR m_swapchain;
        vk::Extent2D m_swapchainExtents;
        std::vector<vk::Image> m_swapchainImages;
        vk::ImageCopy m_swapchainImageCopyRegion{
            vk::ImageSubresourceLayers{
                vk::ImageAspectFlagBits::eColor,
                0,
                0,
                1,
            },
            vk::Offset3D(0, 0, 0),
            vk::ImageSubresourceLayers{
                vk::ImageAspectFlagBits::eColor,
                0,
                0,
                1,
            },
            vk::Offset3D(0, 0, 0),
            vk::Extent3D(m_swapchainExtents, 1), // This extent member is the only one expected to be modifie
        };

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
        [[nodiscard]] bool chooseSwapchainSettings();

        /// Initialize m_device
        void initLogicalDevice();

        // -- end ctor helper functions --

        // -- swapchain creations helpers --

        void cleanupOldSwapchain();
        void initializeNewSwapchain();

        // -- end swapchain creation helpers --

        /// Destroy all previously created command pools
        void cleanupCommandPools();
    };

}
