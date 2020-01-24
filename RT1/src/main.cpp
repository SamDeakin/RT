
#include "Window.hpp"
#include "WindowedRenderer.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

const char* DESIRED_INSTANCE_EXTENSIONS[] = {
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
};

const char* DESIRED_INSTANCE_LAYERS[] = {
    "VK_LAYER_LUNARG_standard_validation", // This includes a standard set of other layers in an optimal order
};

const char* DESIRED_DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
    VK_NV_RAY_TRACING_EXTENSION_NAME,
};

int
main() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize glfw!");
    }

    if (!glfwVulkanSupported()) {
        throw std::runtime_error("Missing glfw Vulkan support!");
    }

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // No features required
    vk::PhysicalDeviceFeatures features{};

    Core::WindowedRenderer<Core::Window> renderer(glfwExtensionCount,
                                                  glfwExtensions,
                                                  sizeof(DESIRED_INSTANCE_EXTENSIONS) / sizeof(decltype(*DESIRED_INSTANCE_EXTENSIONS)),
                                                  DESIRED_INSTANCE_EXTENSIONS,
                                                  sizeof(DESIRED_INSTANCE_LAYERS) / sizeof(decltype(*DESIRED_INSTANCE_LAYERS)),
                                                  DESIRED_INSTANCE_LAYERS,
                                                  sizeof(DESIRED_DEVICE_EXTENSIONS) / sizeof(decltype(*DESIRED_DEVICE_EXTENSIONS)),
                                                  DESIRED_DEVICE_EXTENSIONS,
                                                  features);

    std::cout << "Waiting for keypress to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();
}
