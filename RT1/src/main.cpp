#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

#include "Renderer.hpp"

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

bool
check_device_queue_families(vk::PhysicalDevice& device) {
    std::cout << "    Queue Families:" << std::endl;
    std::vector<vk::QueueFamilyProperties2> queueFamilies = device.getQueueFamilyProperties2();
    bool graphicsFound = false;
    for (auto& queueFamily : queueFamilies) {
        std::cout << "        " << to_string(queueFamily.queueFamilyProperties.queueFlags);
        std::cout << " " << queueFamily.queueFamilyProperties.queueCount;

        if (queueFamily.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics) {
            graphicsFound = true;
        }

        std::cout << std::endl;
    }

    return graphicsFound;
}

vk::PhysicalDevice
find_device(vk::Instance& instance) {
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
        throw std::runtime_error("No physical devices found!");
    }

    for (auto device : physicalDevices) {
        // We just pick the first device that satisfies our requirements
        if (check_physical_device(device)) {
            return device;
        }
    }

    throw std::runtime_error("No suitable devices found!");
}

std::vector<vk::DeviceQueueCreateInfo>
select_queues(vk::PhysicalDevice& device) {
    std::vector<vk::DeviceQueueCreateInfo> queues;
    std::vector<vk::QueueFamilyProperties2> queueFamilies = device.getQueueFamilyProperties2();

    uint32_t index = 0;
    for (auto& family : queueFamilies) {
        if (family.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics) {
            const float priority = 1.0;
            queues.push_back(vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), index, 1, &priority));
            return queues;
        }
        index++;
    }

    throw std::runtime_error("Can't find graphics queue to create?");
}

vk::Device
init_device(vk::Instance& instance) {
    vk::PhysicalDevice physicalDevice = find_device(instance);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = select_queues(physicalDevice);
    vk::PhysicalDeviceFeatures features = init_device_features(physicalDevice);

    vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(),
                                          static_cast<uint32_t>(queueCreateInfos.size()),
                                          queueCreateInfos.data(),
                                          0,
                                          nullptr,
                                          sizeof(DESIRED_DEVICE_EXTENSIONS) / sizeof(decltype(*DESIRED_DEVICE_EXTENSIONS)),
                                          DESIRED_DEVICE_EXTENSIONS,
                                          &features);

    return physicalDevice.createDevice(deviceCreateInfo);
}

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

    Core::Renderer renderer(glfwExtensionCount,
                            glfwExtensions,
                            sizeof(DESIRED_INSTANCE_EXTENSIONS) / sizeof(decltype(*DESIRED_INSTANCE_EXTENSIONS)),
                            DESIRED_INSTANCE_EXTENSIONS,
                            sizeof(DESIRED_INSTANCE_LAYERS) / sizeof(decltype(*DESIRED_INSTANCE_LAYERS)),
                            DESIRED_INSTANCE_LAYERS,
                            sizeof(DESIRED_DEVICE_EXTENSIONS) / sizeof(decltype(*DESIRED_DEVICE_EXTENSIONS)),
                            DESIRED_DEVICE_EXTENSIONS);
}
