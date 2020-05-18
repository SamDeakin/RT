#pragma once

#include "Core/Renderer.hpp"
#include "Core/V1WindowBase.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <memory>
#include <thread>

namespace Core {

    /**
     * A renderer that displays to the user in a window.
     * The user should implement the program behaviour as a subclass of App.
     * The base Core::Window class can be used for W, but can also be subclassed.
     * Passing the classes as template parameters ensures the correct construction order.
     * The extra parameters must inherit from App::Parameters to pass the width and height to the window.
     * @tparam W An implementation of the Core::Window class
     * @tparam A An implementation of the Core::App class
     * @tparam P A struct containing extra parameters used at runtime, passed to the app
     */
    template<class W, class A, class P = typename A::Parameters>
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
                         vk::PhysicalDeviceFeatures features,
                         P& runtimeParameters);

        ~WindowedRenderer() override;

        /**
         * Begin to run the application
         */
        void run();

    private:
        /// Window and display configuration
        std::unique_ptr<W> m_window;

        /// The thread that will setup and begin rendering. Has the same lifetime as this object.
        std::thread m_mainThread;

        /// Initialize m_window
        void initWindow(int width, int height);

        /// Initialize the window surface
        void initSurface();

        /// Initialize the swapchain, images, and views
        void initSwapchain();

        /// Initialize the application
        void initApp(P& runtimeParameters);
    };

    template<class W, class A, class P>
    WindowedRenderer<W, A, P>::WindowedRenderer(uint32_t glfwExtensionCount,
                                                const char** glfwExtensions,
                                                uint32_t instanceExtensionCount,
                                                const char** instanceExtensions,
                                                uint32_t instanceLayerCount,
                                                const char** instanceLayers,
                                                uint32_t deviceExtensionCount,
                                                const char** deviceExtensions,
                                                vk::PhysicalDeviceFeatures features,
                                                P& runtimeParameters) {
        initInstance(glfwExtensionCount, glfwExtensions, instanceExtensionCount, instanceExtensions, instanceLayerCount, instanceLayers);
        initWindow(runtimeParameters.width, runtimeParameters.height);
        initSurface();
        initPhysicalDevice(deviceExtensionCount, deviceExtensions, features);
        initLogicalDevice();
        initSwapchain();
        initApp(runtimeParameters);

        // At this point the WindowedRenderer has been completely setup for rendering.
        // This call must be last as this thread may begin at any point from now on, and the starting
        // thread can no longer access members without synchronization.
        m_mainThread = std::thread(&WindowedRenderer::run, this);
    }

    template<class W, class A, class P>
    WindowedRenderer<W, A, P>::~WindowedRenderer() {
        // Must happen first in dtor, to wait for the thread to join before the rest is destroyed.
        m_mainThread.join();
    }

    template<class W, class A, class P>
    void WindowedRenderer<W, A, P>::initWindow(int width, int height) {
        m_window = std::make_unique<W>(*this, width, height);
    }

    template<class W, class A, class P>
    void WindowedRenderer<W, A, P>::initSurface() {
        GLFWwindow* nativeWindow = m_window->getNativeWindow();

        VkSurfaceKHR surface = {};
        VkResult result = glfwCreateWindowSurface(m_instance, nativeWindow, nullptr, &surface);
        if (result) {
            throw std::runtime_error("Failed to create window surface! " + std::to_string(result));
        }

        m_surface = surface;
    }

    template<class W, class A, class P>
    void WindowedRenderer<W, A, P>::initSwapchain() {
        recreateSwapChain(m_window->getViewportExtents());
    }

    template<class W, class A, class P>
    void WindowedRenderer<W, A, P>::initApp(P& runtimeParameters) {
        std::shared_ptr<A> app = std::make_shared<A>(*this, runtimeParameters);
        m_window->setApp(app);
    }

    template<class W, class A, class P>
    void WindowedRenderer<W, A, P>::run() {
        m_window->run();
        m_device.waitIdle(); // Wait for the device to finish before we try cleaning things up
    }
}
