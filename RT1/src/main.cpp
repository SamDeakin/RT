#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

#include <iostream>
#include <vector>

const char* DESIRED_INSTANCE_EXTENSIONS[] = {
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
};

std::vector<const char*> get_instance_extensions(const char** glfwExtensions, const uint32_t glfwExtensionCount) {
    std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();

    std::vector<const char*> extensionNames;
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        extensionNames.push_back(glfwExtensions[i]);
    }

    std::size_t desiredExtensionCount = sizeof(DESIRED_INSTANCE_EXTENSIONS) / sizeof(const char*);
    // A bitmask initialized to false, to check off extensions as we find them
    std::vector<bool> foundExtensions(desiredExtensionCount, false);

    std::cout << "Supported Instance Extensions:" << std::endl;
    for (auto extension : extensionProperties) {
        std::cout << "    ";

        for (std::size_t i = 0; i < desiredExtensionCount; i++) {
            if (!strcmp(DESIRED_INSTANCE_EXTENSIONS[i], extension.extensionName)) {
                // Push the static name, the other may be destroyed when we are done with the extension list.
                extensionNames.push_back(DESIRED_INSTANCE_EXTENSIONS[i]);
                foundExtensions[i] = true;
                std::cout << "[Enabled] ";
            }
        }

        std::cout << extension.extensionName << std::endl;
    }

    // Make sure all desired extensions were found
    for (std::size_t i = 0; i < desiredExtensionCount; i++) {
        if (!foundExtensions[i]) {
            std::cout << "    [Missing] " << DESIRED_INSTANCE_EXTENSIONS[i] << std::endl;
            throw std::runtime_error("Failed to find all desired instance extensions!");
        }
    }

    return extensionNames;
}

const char* DESIRED_INSTANCE_LAYERS[] = {
        "VK_LAYER_LUNARG_standard_validation", // This includes a standard set of other layers in an optimal order
};

std::vector<const char*> get_instance_layers() {
    std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
    std::vector<const char*> layerNames;

    std::size_t desiredLayerCount = sizeof(DESIRED_INSTANCE_LAYERS) / sizeof(const char*);
    // A bitmask initialized to false, to check off extensions as we find them
    std::vector<bool> foundLayers(desiredLayerCount, false);

    std::cout << "Supported Instance Layers:" << std::endl;
    for (auto layer : layerProperties) {
        std::cout << "    ";

        for (std::size_t i = 0; i < desiredLayerCount; i++) {
            if (!strcmp(DESIRED_INSTANCE_LAYERS[i], layer.layerName)) {
                // Push the static name, the other may be destroyed when we are done with the extension list.
                layerNames.push_back(DESIRED_INSTANCE_LAYERS[i]);
                foundLayers[i] = true;
                std::cout << "[Enabled] ";
            }
        }

        std::cout << layer.layerName << std::endl;
    }

    // We don't error check missing layers: they may just not be installed
    for (std::size_t i = 0; i < desiredLayerCount; i++) {
        if (!foundLayers[i]) {
            std::cout << "    [Missing] " << DESIRED_INSTANCE_EXTENSIONS[i] << std::endl;
        }
    }

    return layerNames;
}

void init_vulkan(vk::Instance& instance) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize glfw!");
    }

    if (!glfwVulkanSupported()) {
        throw std::runtime_error("Missing glfw Vulkan support!");
    }

    vk::ApplicationInfo appInfo = {};
    appInfo.pApplicationName = "RT1";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "TestEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> allExtensions = get_instance_extensions(glfwExtensions, glfwExtensionCount);

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
    std::vector<const char*> layers = get_instance_layers();
    instanceCreateInfo.enabledLayerCount = layers.size();
    instanceCreateInfo.ppEnabledLayerNames = layers.data();
#endif

    vk::Result result = vk::createInstance(&instanceCreateInfo, nullptr, &instance);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}

void cleanup(vk::Instance& instance) {
    instance.destroy();
}

int main() {
    vk::Instance instance;
    init_vulkan(instance);

    cleanup(instance);
}