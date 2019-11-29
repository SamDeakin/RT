#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

#include <iostream>

void init_vulkan() {
    if (!glfwInit()) {
        std::cout << ":(" << std::endl;
    }

    if (!glfwVulkanSupported()) {
        std::cout << ":((" << std::endl;
    }

    vk::ApplicationInfo appInfo = {};
    appInfo.pApplicationName = "RT1";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "TestEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vk::InstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;
    instanceCreateInfo.enabledLayerCount = 0;

    vk::Instance instance;
    vk::Result result = vk::createInstance(&instanceCreateInfo, nullptr, &instance);
    if (result != vk::Result::eSuccess) {
        std::cout << ":(((" << std::endl;
    } else {
        std::cout << ":)" << std::endl;
    }
}

int main() {
    init_vulkan();
}