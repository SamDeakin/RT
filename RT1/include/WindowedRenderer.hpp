#pragma once

#include "Renderer.hpp"
#include "Window.hpp"

//#include <concepts>

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
        WindowedRenderer(uint32_t glfwExtensionCount,
                         const char** glfwExtensions,
                         uint32_t instanceExtensionCount,
                         const char** instanceExtensions,
                         uint32_t instanceLayerCount,
                         const char** instanceLayers,
                         uint32_t deviceExtensionCount,
                         const char** deviceExtensions,
                         vk::PhysicalDeviceFeatures features)
            : Renderer(glfwExtensionCount,
                       glfwExtensions,
                       instanceExtensionCount,
                       instanceExtensions,
                       instanceLayerCount,
                       instanceLayers,
                       deviceExtensionCount,
                       deviceExtensions,
                       features)
            , m_window(*this) {}
        ~WindowedRenderer() = default;

    private:
        W m_window;
    };

}
