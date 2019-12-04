#include "Renderer.hpp"

#include <iostream>
#include <string>
#include <unordered_set>

namespace Core {

    // -- ctor and helpers --

    Renderer::Renderer(uint32_t glfwExtensionCount,
                       const char const** glfwExtensions,
                       uint32_t instanceExtensionCount,
                       const char const** instanceExtensions,
                       uint32_t instanceLayerCount,
                       const char const** instanceLayers,
                       uint32_t deviceExtensionCount,
                       const char const** deviceExtensions) {
        initInstance(glfwExtensionCount, glfwExtensions, instanceExtensionCount, instanceExtensions, instanceLayerCount, instanceLayers);
        initDevice(deviceExtensionCount, deviceExtensions);

        // TODO
    }

    void Renderer::initInstance(uint32_t glfwExtensionCount,
                                const char const** glfwExtensions,
                                uint32_t instanceExtensionCount,
                                const char const** instanceExtensions,
                                uint32_t instanceLayerCount,
                                const char const** instanceLayers) {
        addInstanceExtensions(glfwExtensionCount, glfwExtensions, instanceExtensionCount, instanceExtensions);
        addInstanceLayers(instanceLayerCount, instanceLayers);

        // TODO
    }

    void Renderer::addInstanceExtensions(uint32_t glfwExtensionCount,
                                         const char const** glfwExtensions,
                                         uint32_t instanceExtensionCount,
                                         const char const** instanceExtensions) {
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
                std::cout << "    [Missing] " << extensionName << std::endl;
                throw std::runtime_error("Failed to find all desired instance extensions!");
            }
        }
    }

    void Renderer::addInstanceLayers(uint32_t instanceLayerCount, const char const** instanceLayers) {
        m_instanceLayers.reserve(instanceLayerCount);
        // TODO
    }

    void Renderer::initDevice(uint32_t deviceExtensionCount, const char const** deviceExtensions) {
        addDeviceExtensions(deviceExtensionCount, deviceExtensions);

        // TODO
    }

    void Renderer::addDeviceExtensions(uint32_t deviceExtensionCount, const char const** deviceExtensions) {
        m_deviceExtensions.reserve(deviceExtensionCount);
        // TODO
    }

    // -- end ctor and helpers --

    Renderer::~Renderer() {
        m_device.destroy();
        m_instance.destroy();
    }

}