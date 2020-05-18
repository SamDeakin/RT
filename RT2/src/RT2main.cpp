
#include "RT2/RT2App.hpp"

#include <Core/V2WindowBase.hpp>
#include <Core/WindowedRenderer.hpp>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <iostream>

const char* DESIRED_INSTANCE_EXTENSIONS[] = {
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
};

const char* DESIRED_INSTANCE_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation", // This includes a standard set of other layers in an optimal order
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

    Core::V2AppBase::Parameters parameters{
        1920,
        1080,
    };

    Core::WindowedRenderer<Core::V2WindowBase, RT2::RT2App> renderer(glfwExtensionCount,
                                                               glfwExtensions,
                                                               std::size(DESIRED_INSTANCE_EXTENSIONS),
                                                               DESIRED_INSTANCE_EXTENSIONS,
                                                               std::size(DESIRED_INSTANCE_LAYERS),
                                                               DESIRED_INSTANCE_LAYERS,
                                                               std::size(DESIRED_DEVICE_EXTENSIONS),
                                                               DESIRED_DEVICE_EXTENSIONS,
                                                               features,
                                                               parameters);
}
