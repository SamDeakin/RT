#include "Renderer.hpp"

#include <iostream>
#include <string>
#include <unordered_set>

namespace Core {

    // -- ctor and helpers --

    Renderer::Renderer(uint32_t glfwExtensionCount,
                       const char** glfwExtensions,
                       uint32_t instanceExtensionCount,
                       const char** instanceExtensions,
                       uint32_t instanceLayerCount,
                       const char** instanceLayers,
                       uint32_t deviceExtensionCount,
                       const char** deviceExtensions,
                       vk::PhysicalDeviceFeatures features) {
        initInstance(glfwExtensionCount, glfwExtensions, instanceExtensionCount, instanceExtensions, instanceLayerCount, instanceLayers);
        initPhysicalDevice(deviceExtensionCount, deviceExtensions, features);
        initLogicalDevice();

        // TODO
    }

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
        for (vk::ExtensionProperties extension : m_instanceExtensions) {
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
        std::cout << "    Queue Families:" << std::endl;
        for (auto& family : m_deviceQueueFamilies) {
            std::cout << "        " << to_string(family.queueFlags);
            std::cout << " " << family.queueCount << std::endl;

            if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsFound = true;
            }
        }

        if (!graphicsFound) {
            std::cout << "        No graphics queue found" << std::endl;
        } else {
            std::cout << "        Graphics queue found" << std::endl;
        }

        return graphicsFound;
    }

    void Renderer::initLogicalDevice() {
        std::vector<uint32_t> queuesDesiredPerFamily(m_deviceQueueFamilies.size(), 0u);
        uint32_t graphicsQueue, transferQueue, computeQueue;
        uint32_t graphicsQueueCount = 0, transferQueueCount = 0, computeQueueCount = 0;

        // The first queue with graphics is chosen for graphics
        // The first queue with only transfer is chosen for transfer, and the same for compute
        bool graphicsFound = false;
        bool transferFound = false;
        bool computeFound = false;
        for (uint32_t i = 0; i < m_deviceQueueFamilies.size(); i++) {
            if (!graphicsFound && m_deviceQueueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsQueue = i;
                graphicsQueueCount = m_deviceQueueFamilies[i].queueCount;
                queuesDesiredPerFamily[i] = m_deviceQueueFamilies[i].queueCount;
                graphicsFound = true;
            } else if (!transferFound && m_deviceQueueFamilies[i].queueFlags == vk::QueueFlagBits::eTransfer) {
                transferQueue = i;
                transferQueueCount = m_deviceQueueFamilies[i].queueCount;
                queuesDesiredPerFamily[i] = m_deviceQueueFamilies[i].queueCount;
                transferFound = true;
            } else if (!computeFound && m_deviceQueueFamilies[i].queueFlags == vk::QueueFlagBits::eCompute) {
                computeQueue = i;
                computeQueueCount = m_deviceQueueFamilies[i].queueCount;
                queuesDesiredPerFamily[i] = m_deviceQueueFamilies[i].queueCount;
                computeFound = true;
            }
        }

        if (!graphicsFound || !transferFound || !computeFound)
            throw std::runtime_error("Couldn't find all desired unique queue types within device");

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
        std::vector<std::vector<float>> queuePrioritiesLists;
        for (uint32_t i = 0; i < queuesDesiredPerFamily.size(); i++) {
            if (queuesDesiredPerFamily[i] > 0) {
                std::vector<float> queuePriorities(queuesDesiredPerFamily[i], 1.0 / queuesDesiredPerFamily[i]);

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

        for (uint32_t queueIndex = 0; queueIndex < graphicsQueueCount; queueIndex++) {
            m_graphicsQueues.emplace_back(std::move(m_device.getQueue(graphicsQueue, queueIndex)));
        }
        for (uint32_t queueIndex = 0; queueIndex < transferQueueCount; queueIndex++) {
            m_transferQueues.emplace_back(std::move(m_device.getQueue(transferQueue, queueIndex)));
        }
        for (uint32_t queueIndex = 0; queueIndex < computeQueueCount; queueIndex++) {
            m_computeQueues.emplace_back(std::move(m_device.getQueue(computeQueue, queueIndex)));
        }
    }

    // -- end ctor and helpers --

    Renderer::~Renderer() noexcept {
        m_device.destroy();
        m_instance.destroy();
    }

}