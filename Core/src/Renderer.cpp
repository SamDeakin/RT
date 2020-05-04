#include "Core/Renderer.hpp"

#include <iostream>
#include <set>
#include <string>
#include <unordered_set>

#define DESIRED_PRESENT_QUEUES 1

namespace Core {

    // -- ctor and helpers --

    void Renderer::initInstance(uint32_t glfwExtensionCount,
                                const char** glfwExtensions,
                                uint32_t instanceExtensionCount,
                                const char** instanceExtensions,
                                uint32_t instanceLayerCount,
                                const char** instanceLayers) {
        addInstanceExtensions(glfwExtensionCount, glfwExtensions, instanceExtensionCount, instanceExtensions);
        addInstanceLayers(instanceLayerCount, instanceLayers);

        vk::ApplicationInfo appInfo{
            "RT1",
            VK_MAKE_VERSION(1, 0, 0),
            "TestEngine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_1,
        };

        std::vector<const char*> allExtensions;
        for (vk::ExtensionProperties& extension : m_instanceExtensions) {
            allExtensions.push_back(extension.extensionName);
        }

        vk::InstanceCreateInfo instanceCreateInfo{
            vk::InstanceCreateFlags(),
            &appInfo,
            0,
            nullptr,
            static_cast<uint32_t>(allExtensions.size()),
            allExtensions.data(),
        };

#ifdef _DEBUG
        // Since layers generally just add debug info we don't want to add them outside of debug
        std::vector<const char*> layers;
        for (vk::LayerProperties layer : m_instanceLayers) {
            layers.push_back(layer.layerName);
        }
        instanceCreateInfo.enabledLayerCount = layers.size();
        instanceCreateInfo.ppEnabledLayerNames = layers.data();
#endif

        vk::Result result = vk::createInstance(&instanceCreateInfo, nullptr, &m_instance);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }
    }

    void Renderer::addInstanceExtensions(uint32_t glfwExtensionCount,
                                         const char** glfwExtensions,
                                         uint32_t instanceExtensionCount,
                                         const char** instanceExtensions) {
        m_instanceExtensions.reserve(glfwExtensionCount + instanceExtensionCount);

        // Build a set of all extensions we are searching for.
        // Remove them as we find them, so we can track whether we have found them all.
        // This also handles duplicates between user extensions and glfw extensions.
        std::unordered_set<std::string> requiredExtensions;
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace(glfwExtensions[i]);
        }
        for (uint32_t i = 0; i < instanceExtensionCount; i++) {
            requiredExtensions.emplace(instanceExtensions[i]);
        }

        std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
        std::cout << "Supported Instance Extensions:" << std::endl;
        for (auto& extension : extensionProperties) {
            std::cout << "    ";

            auto search = requiredExtensions.find(std::string(extension.extensionName));
            if (search != requiredExtensions.end()) {
                m_instanceExtensions.push_back(extension);
                requiredExtensions.erase(search);
                std::cout << "[Enabled] ";
            }

            std::cout << extension.extensionName << std::endl;
        }

        // Make sure all desired extensions were found
        if (!requiredExtensions.empty()) {
            for (const std::string& extensionName : requiredExtensions) {
                std::cout << "    [Failed] " << extensionName << std::endl;
            }
            throw std::runtime_error("Failed to find all desired instance extensions!");
        }
    }

    void Renderer::addInstanceLayers(uint32_t instanceLayerCount, const char** instanceLayers) {
        m_instanceLayers.reserve(instanceLayerCount);

        std::unordered_set<std::string> requestedLayers;
        for (uint32_t i = 0; i < instanceLayerCount; i++) {
            requestedLayers.emplace(instanceLayers[i]);
        }

        std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
        std::cout << "Supported Instance Layers:" << std::endl;
        for (auto& layer : layerProperties) {
            std::cout << "    ";

            auto search = requestedLayers.find(std::string(layer.layerName));
            if (search != requestedLayers.end()) {
                // Push the static name, the other may be destroyed when we are done with the extension list.
                m_instanceLayers.push_back(layer);
                requestedLayers.erase(search);
                std::cout << "[Enabled] ";
            }

            std::cout << layer.layerName << std::endl;
        }

        // We don't error check missing layers: they may just not be installed
        if (!requestedLayers.empty()) {
            for (const std::string& layerName : requestedLayers) {
                std::cout << "    [Missing] " << layerName << std::endl;
            }
        }
    }

    void Renderer::initPhysicalDevice(uint32_t deviceExtensionCount, const char** deviceExtensions, vk::PhysicalDeviceFeatures features) {
        std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
        if (physicalDevices.empty()) {
            throw std::runtime_error("No physical devices found!");
        }

        bool deviceFound = false;
        for (auto& device : physicalDevices) {
            m_physicalDevice = device;

            // Run every check so that all debug reasons for not selecting a device will be printed
            bool suitable = true;
            suitable &= addDeviceProperties();
            suitable &= addDeviceFeatures(features);
            suitable &= addDeviceExtensions(deviceExtensionCount, deviceExtensions);
            suitable &= addDeviceQueues();
            suitable &= chooseSwapchainSettings();

            if (suitable) {
                deviceFound = true;
                break;
            }
        }

        if (!deviceFound) {
            throw std::runtime_error("No suitable devices found!");
        }
    }

    bool Renderer::addDeviceProperties() {
        m_deviceProperties = m_physicalDevice.getProperties();
        std::cout << "Checking device: " << m_deviceProperties.deviceName << std::endl;

        if (m_deviceProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu) {
            std::cout << "    [Incorrect] Not a discrete GPU" << std::endl;
            return false;
        }

        return true;
    }

    bool Renderer::addDeviceFeatures(vk::PhysicalDeviceFeatures features) {
        m_features = features;
        return true;
    }

    bool Renderer::addDeviceExtensions(uint32_t deviceExtensionCount, const char** deviceExtensions) {
        m_deviceExtensions.reserve(deviceExtensionCount);

        std::unordered_set<std::string> requiredExtensions;
        for (uint32_t i = 0; i < deviceExtensionCount; i++) {
            requiredExtensions.emplace(deviceExtensions[i]);
        }

        std::vector<vk::ExtensionProperties> extensionProperties = m_physicalDevice.enumerateDeviceExtensionProperties();
        std::cout << "    Supported Device Extensions:" << std::endl;
        for (auto& extension : extensionProperties) {
            std::cout << "        ";

            auto search = requiredExtensions.find(std::string(extension.extensionName));
            if (search != requiredExtensions.end()) {
                m_deviceExtensions.push_back(extension);
                requiredExtensions.erase(search);
                std::cout << "[Enabled] ";
            }

            std::cout << extension.extensionName << std::endl;
        }

        // Make sure all desired extensions were found
        if (!requiredExtensions.empty()) {
            for (const std::string& extensionName : requiredExtensions) {
                std::cout << "        [Missing] " << extensionName << std::endl;
            }
            return false;
        }
        return true;
    }

    bool Renderer::addDeviceQueues() {
        m_deviceQueueFamilies = m_physicalDevice.getQueueFamilyProperties();

        bool graphicsFound = false;
        bool presentFound = false;
        std::cout << "    Queue Families:" << std::endl;
        for (uint32_t familyIndex = 0; familyIndex < m_deviceQueueFamilies.size(); familyIndex++) {
            auto& family = m_deviceQueueFamilies[familyIndex];
            std::cout << "        " << to_string(family.queueFlags);
            std::cout << " " << family.queueCount;

            if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsFound = true;
            }

            if (m_physicalDevice.getSurfaceSupportKHR(familyIndex, m_surface)) {
                presentFound = true;
                std::cout << " [Present]";
            }

            std::cout << std::endl;
        }

        if (!graphicsFound) {
            std::cout << "        No graphics queue found" << std::endl;
        } else {
            std::cout << "        Graphics queue found" << std::endl;
        }

        if (!presentFound) {
            std::cout << "        No present queue found" << std::endl;
        } else {
            std::cout << "        Present queue found" << std::endl;
        }

        return graphicsFound;
    }

    void Renderer::initLogicalDevice() {
        std::vector<uint32_t> queuesDesiredPerFamily(m_deviceQueueFamilies.size(), 0u);
        uint32_t graphicsQueueCount = 0;
        uint32_t presentQueueCount = 0;
        uint32_t transferQueueCount = 0;
        uint32_t computeQueueCount = 0;
        QueueGroup graphicsQueueGroup{.primaryType = QueueType::Graphics};
        QueueGroup presentQueueGroup{.primaryType = QueueType::Present};
        QueueGroup transferQueueGroup{.primaryType = QueueType::Transfer};
        QueueGroup computeQueueGroup{.primaryType = QueueType::Compute};
        // TODO A fallback sparse binding queue (By default should use the next queue to use results)

        // The first queue with graphics is chosen for graphics
        // The first queue with transfer and not graphics is chosen for transfer, and the same for compute
        bool graphicsFound = false;
        bool presentFound = false;
        bool transferFound = false;
        bool computeFound = false;
        for (uint32_t i = 0; i < m_deviceQueueFamilies.size(); i++) {
            if (!graphicsFound && m_deviceQueueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsQueueGroup.familyIndex = i;
                graphicsQueueGroup.supportedTypes = m_deviceQueueFamilies[i].queueFlags;
                graphicsQueueCount = m_deviceQueueFamilies[i].queueCount;
                queuesDesiredPerFamily[i] = graphicsQueueCount;
                graphicsFound = true;
            }
            if (!transferFound && m_deviceQueueFamilies[i].queueFlags & vk::QueueFlagBits::eTransfer &&
                !(m_deviceQueueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
                transferQueueGroup.familyIndex = i;
                transferQueueGroup.supportedTypes = m_deviceQueueFamilies[i].queueFlags;
                transferQueueCount = m_deviceQueueFamilies[i].queueCount;
                queuesDesiredPerFamily[i] = transferQueueCount;
                transferFound = true;
            }
            if (!computeFound && m_deviceQueueFamilies[i].queueFlags & vk::QueueFlagBits::eCompute &&
                !(m_deviceQueueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
                computeQueueGroup.familyIndex = i;
                computeQueueGroup.supportedTypes = m_deviceQueueFamilies[i].queueFlags;
                computeQueueCount = m_deviceQueueFamilies[i].queueCount;
                queuesDesiredPerFamily[i] = computeQueueCount;
                computeFound = true;
            }

            // We steal the first few queues from whatever queue can present later, as it is probably
            // shared with the graphics queue
            if (!presentFound && m_physicalDevice.getSurfaceSupportKHR(i, m_surface)) {
                presentQueueGroup.familyIndex = i;
                presentQueueGroup.supportedTypes = m_deviceQueueFamilies[i].queueFlags;
                presentQueueCount = DESIRED_PRESENT_QUEUES;
                presentFound = true;

                if (graphicsFound && graphicsQueueGroup.familyIndex == presentQueueGroup.familyIndex) {
                    graphicsQueueCount -= presentQueueCount; // Assumes that queue usage is exclusive (its not)
                } else {
                    queuesDesiredPerFamily[i] = presentQueueCount;
                }
            }
        }

        if (!graphicsFound || !presentFound || !transferFound || !computeFound)
            // TODO Split existing families into groups in this case
            throw std::runtime_error("Couldn't find all desired unique queue types within device");

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
        std::vector<std::vector<float>> queuePrioritiesLists;
        for (uint32_t i = 0; i < queuesDesiredPerFamily.size(); i++) {
            if (queuesDesiredPerFamily[i] > 0) {
                std::vector<float> queuePriorities(queuesDesiredPerFamily[i], 1.0 / queuesDesiredPerFamily[i]);

                // TODO Adjust queue priorities for the present queues if they are starved by the graphics queues in practice

                queueCreateInfo.emplace_back(vk::DeviceQueueCreateFlags(), i, queuesDesiredPerFamily[i], queuePriorities.data());

                // Move onto the list of lists to prevent the data from being freed
                queuePrioritiesLists.emplace_back(std::move(queuePriorities));
            }
        }

        std::vector<const char*> deviceExtensionNames;
        for (auto& extension : m_deviceExtensions) {
            deviceExtensionNames.push_back(extension.extensionName);
        }

        vk::DeviceCreateInfo deviceCreateInfo{
            vk::DeviceCreateFlags(),
            static_cast<uint32_t>(queueCreateInfo.size()),
            queueCreateInfo.data(),
            0,
            nullptr,
            static_cast<uint32_t>(deviceExtensionNames.size()),
            deviceExtensionNames.data(),
            &m_features,
        };

        m_device = m_physicalDevice.createDevice(deviceCreateInfo);

        // The present queues take first, in case that matters
        for (uint32_t queueIndex = 0; queueIndex < presentQueueCount; queueIndex++) {
            presentQueueGroup.queues.emplace_back(m_device.getQueue(presentQueueGroup.familyIndex, queueIndex));
        }
        m_queues[QueueType::Present] = std::move(presentQueueGroup);

        for (uint32_t queueIndex = 0; queueIndex < graphicsQueueCount; queueIndex++) {
            graphicsQueueGroup.queues.emplace_back(m_device.getQueue(graphicsQueueGroup.familyIndex, queueIndex));
        }
        m_queues[QueueType::Graphics] = std::move(graphicsQueueGroup);

        for (uint32_t queueIndex = 0; queueIndex < transferQueueCount; queueIndex++) {
            transferQueueGroup.queues.emplace_back(m_device.getQueue(transferQueueGroup.familyIndex, queueIndex));
        }
        m_queues[QueueType::Transfer] = std::move(transferQueueGroup);

        for (uint32_t queueIndex = 0; queueIndex < computeQueueCount; queueIndex++) {
            computeQueueGroup.queues.emplace_back(m_device.getQueue(computeQueueGroup.familyIndex, queueIndex));
        }
        m_queues[QueueType::Compute] = std::move(computeQueueGroup);
    }

    bool Renderer::chooseSwapchainSettings() {
        std::cout << "    Surface Colour Formats:" << std::endl;
        std::vector<vk::SurfaceFormatKHR> supportedFormats = m_physicalDevice.getSurfaceFormatsKHR(m_surface);
        // Simple impl, just choose 8-bit format with srgb colour space
        // Mostly cause I'm a scrub that doesn't have any HDR devices lol
        bool formatFound = false;
        for (auto& format : supportedFormats) {
            std::cout << "        ";

            // BGR appears to be more commonly supported than RGB
            if (!formatFound && format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                formatFound = true;
                m_surfaceFormat = format;
                std::cout << "[Selected] ";
            }

            std::cout << vk::to_string(format.format) << " | " << vk::to_string(format.colorSpace) << std::endl;
        }
        if (!formatFound) {
            std::cout << "        [Missing] " << vk::to_string(vk::Format::eB8G8R8A8Unorm) << " | " << vk::to_string(vk::ColorSpaceKHR::eSrgbNonlinear)
                      << std::endl;
            return false;
        }

        std::vector<vk::PresentModeKHR> supportedPresentModes = m_physicalDevice.getSurfacePresentModesKHR(m_surface);
        std::set<vk::PresentModeKHR> presentModeSet(supportedPresentModes.begin(), supportedPresentModes.end());
        m_presentMode = vk::PresentModeKHR::eFifo; // Guaranteed to be supported
        std::vector<vk::PresentModeKHR> desiredPresentModes{
            vk::PresentModeKHR::eMailbox,
            vk::PresentModeKHR::eImmediate,
            vk::PresentModeKHR::eFifoRelaxed,
        };

        for (auto& mode : desiredPresentModes) {
            if (presentModeSet.count(mode)) {
                m_presentMode = mode;
                break;
            }
        }

        std::cout << "    Swapchain Present Modes:" << std::endl;
        for (auto& mode : supportedPresentModes) {
            std::cout << "        ";
            if (mode == m_presentMode) {
                std::cout << "[Selected] ";
            }

            std::cout << vk::to_string(mode) << std::endl;
        }

        return true;
    }

    // -- end ctor and helpers --

    Renderer::~Renderer() noexcept {
        cleanupOldSwapchain();
        m_instance.destroySurfaceKHR(m_surface);
        m_device.destroy();
        m_instance.destroy();
    }

    vk::Device Renderer::getDevice() const { return m_device; }
    vk::PhysicalDevice Renderer::getPhysicalDevice() const { return m_physicalDevice; }
    vk::Instance Renderer::getInstance() const { return m_instance; }

    const vk::Extent2D& Renderer::getSwapchainExtents() const { return m_swapchainExtents; }
    const std::vector<vk::Image>& Renderer::getSwapchainImages() const { return m_swapchainImages; }
    std::size_t Renderer::getNumSwapchainImages() const { return m_swapchainImages.size(); }
    const vk::Format& Renderer::getOutputFormat() const { return m_surfaceFormat.format; }

    void Renderer::recreateSwapChain(vk::Extent2D windowExtents) {
        vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

        // TODO Decide on min image count
        uint32_t minImageCount = surfaceCapabilities.minImageCount;

        // TODO Decide on extents
        m_swapchainExtents = windowExtents;

        // The number of layers for each swapchain image: probably 1 unless the image is in stereoscopic 3D or something
        uint32_t imageArrayLayers = 1;

        // We only support rendering to texture for now
        // TODO Ensure that the present queue is capable of transfers previously
        // Solid chance it will be, as graphics/compute implies transfer
        vk::ImageUsageFlags imageUsageFlags = vk::ImageUsageFlagBits::eTransferDst;

        vk::SharingMode sharingMode = vk::SharingMode::eExclusive;
        uint32_t sharingQueueCount = 0;
        uint32_t possibleSharingQueues[] = {m_queues[QueueType::Present].familyIndex, m_queues[QueueType::Graphics].familyIndex};
        uint32_t* sharingQueues = nullptr;
        if (possibleSharingQueues[0] == possibleSharingQueues[1]) {
            sharingQueues = possibleSharingQueues;
            sharingQueueCount = 2;
        }

        // No special transform to apply to the surface
        vk::SurfaceTransformFlagBitsKHR surfaceTransform = surfaceCapabilities.currentTransform;

        vk::CompositeAlphaFlagBitsKHR alphaBlend = vk::CompositeAlphaFlagBitsKHR::eOpaque;

        // We want to clip the portions of the window that are drawn to exclude covered portions
        vk::Bool32 clipMode = VK_TRUE;

        vk::SwapchainCreateInfoKHR swapchainCreateInfo{vk::SwapchainCreateFlagsKHR(),
                                                       m_surface,
                                                       minImageCount,
                                                       m_surfaceFormat.format,
                                                       m_surfaceFormat.colorSpace,
                                                       m_swapchainExtents,
                                                       imageArrayLayers,
                                                       imageUsageFlags,
                                                       sharingMode,
                                                       sharingQueueCount,
                                                       sharingQueues,
                                                       surfaceTransform,
                                                       alphaBlend,
                                                       m_presentMode,
                                                       clipMode,
                                                       m_swapchain};

        vk::SwapchainKHR newSwapchain = m_device.createSwapchainKHR(swapchainCreateInfo);
        cleanupOldSwapchain();
        m_swapchain = newSwapchain;
        initializeNewSwapchain();
    }
    void Renderer::cleanupOldSwapchain() {
        m_swapchainImages.clear();
        m_device.destroySwapchainKHR(m_swapchain);
    }
    void Renderer::initializeNewSwapchain() {
        m_swapchainImages = m_device.getSwapchainImagesKHR(m_swapchain);

        // Since we have chosen to implement copy-to-swapchain instead of render-to-swapchain, we do not create
        // ImageViews for the Images.
        // Instead we create the necessary copy-structures for later. This is rigid because we only support copying from
        // a compatible image, and copying the entire image at once. We only update the extents.
        m_swapchainImageCopyRegion.extent = vk::Extent3D(m_swapchainExtents, 1);
    }
    std::vector<std::unique_ptr<GraphicsPipeline>> Renderer::createGraphicsPipelines(uint32_t count, const vk::GraphicsPipelineCreateInfo* createInfos) {
        vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo> createInfoArray(count, createInfos);
        std::vector<vk::Pipeline> createdPipelines = m_device.createGraphicsPipelines(vk::PipelineCache(), createInfoArray);

        std::vector<std::unique_ptr<GraphicsPipeline>> pipelineObjects;
        pipelineObjects.reserve(createdPipelines.size());

        for (vk::Pipeline& nativePipeline : createdPipelines) {
            pipelineObjects.push_back(std::make_unique<GraphicsPipeline>(m_device, nativePipeline));
        }

        return pipelineObjects;
    }
}
