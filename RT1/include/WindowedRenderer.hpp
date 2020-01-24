#pragma once

#include "Renderer.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

//#include <concepts>
#include <memory>

namespace Core {

    /**
     * A renderer that displays to the user in a window.
     * The user should implement the program behaviour as a subclass of Window.
     * @tparam W An implementation of the Core::Window class
     */
    template<class W>
    // TODO useful, enable when concepts has compiler support
    // requires std::derived_from<Window>
    class WindowedRenderer : public Renderer {
    public:
        /**
         * Construct a Renderer with the desired extensions and layers.
         * Throws a std::runtimeException if it cannot be created.
         * @param instanceExtensions: Desired instance extensions
         * @param instanceLayers: Desired instance layers
         * @param deviceExtensions: Desired device extensions
         */
        WindowedRenderer(uint32_t glfwExtensionCount,
                         const char** glfwExtensions,
                         uint32_t instanceExtensionCount,
                         const char** instanceExtensions,
                         uint32_t instanceLayerCount,
                         const char** instanceLayers,
                         uint32_t deviceExtensionCount,
                         const char** deviceExtensions,
                         vk::PhysicalDeviceFeatures features);

        ~WindowedRenderer() override;

    private:
        /// Window and display configuration
        std::unique_ptr<W> m_window;
        vk::SurfaceKHR m_surface;

        /// Initialize m_window
        void initWindow();

        /// Initialize the window surface
        void initSurface();
    };

    template<class W>
    WindowedRenderer<W>::WindowedRenderer(uint32_t glfwExtensionCount,
                                          const char** glfwExtensions,
                                          uint32_t instanceExtensionCount,
                                          const char** instanceExtensions,
                                          uint32_t instanceLayerCount,
                                          const char** instanceLayers,
                                          uint32_t deviceExtensionCount,
                                          const char** deviceExtensions,
                                          vk::PhysicalDeviceFeatures features) {
        initInstance(glfwExtensionCount, glfwExtensions, instanceExtensionCount, instanceExtensions, instanceLayerCount, instanceLayers);
        initWindow();
        initSurface();
        initPhysicalDevice(deviceExtensionCount, deviceExtensions, features);
        initLogicalDevice();
    }

    template<class W>
    WindowedRenderer<W>::~WindowedRenderer() {
        m_instance.destroySurfaceKHR(m_surface);
    }

    template<class W>
    void WindowedRenderer<W>::initWindow() {
        m_window = std::make_unique<W>(*this);
    }

    template<class W>
    void WindowedRenderer<W>::initSurface() {
        GLFWwindow* nativeWindow = m_window->getNativeWindow();

        VkSurfaceKHR surface = {};
        VkResult result = glfwCreateWindowSurface(m_instance, nativeWindow, nullptr, &surface);
        if (result) {
            throw std::runtime_error("Failed to create window surface! " + result);
        }

        m_surface = surface;
    }

}
