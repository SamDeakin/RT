#include "Core/Window.hpp"
#include <iostream>
namespace Core {

    Window::Window(Renderer& renderer, int width, int height)
        : m_renderer(renderer)
        , m_minimized(false) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_nativeWindow = glfwCreateWindow(width, height, "RT", nullptr, nullptr);
        fixTopBar(width, height);

        registerCallbacks();
    }

    Window::~Window() noexcept { glfwDestroyWindow(m_nativeWindow); }

    GLFWwindow* Window::getNativeWindow() { return m_nativeWindow; }

    vk::Extent2D Window::getViewportExtents() {
        int width, height;
        glfwGetWindowSize(m_nativeWindow, &width, &height);

        return vk::Extent2D{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height),
        };
    }

    void Window::fixTopBar(int requestedWidth, int requestedHeight) {
        // We poll events throughout this function to remove spurious window resize events that may appear later
        int width, height;
        glfwGetWindowSize(m_nativeWindow, &width, &height);
        int left, top, right, bottom;
        glfwGetWindowFrameSize(m_nativeWindow, &left, &top, &right, &bottom);

        if (width == requestedWidth && height == requestedHeight)
            return;

        // Attempt to just set the window size first. This may stop some platforms from
        // taking space from the requested size for the top bar.
        glfwSetWindowSize(m_nativeWindow, requestedWidth, requestedHeight);
        glfwPollEvents();

        glfwGetWindowSize(m_nativeWindow, &width, &height);
        if (width == requestedWidth && height == requestedHeight)
            return;

        glfwGetWindowFrameSize(m_nativeWindow, &left, &top, &right, &bottom);

        // Attemp to add the top bar (and other spacing) to the size when requesting
        glfwSetWindowSize(m_nativeWindow, left + width + right, top + height + bottom);
        glfwPollEvents();

        glfwGetWindowSize(m_nativeWindow, &width, &height);
        if (width == requestedWidth && height == requestedHeight)
            return;

        // If that didn't work then just go back to setting the requested size
        glfwSetWindowSize(m_nativeWindow, requestedWidth, requestedHeight);
        glfwPollEvents();
    }

    void Window::registerCallbacks() {
        // In order to re-find this class after, we use the glfw user-pointer feature
        glfwSetWindowUserPointer(m_nativeWindow, this);

        glfwSetFramebufferSizeCallback(m_nativeWindow, [](GLFWwindow* window, int width, int height) {
            Window* userptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            userptr->windowResized(width, height);
        });
    }

    void Window::setApp(App* app) { m_mainApp.reset(app); }

    bool Window::windowResized(int width, int height) {
        if (width == 0 && height == 0) {
            m_minimized = true;
        } else {
            m_minimized = false;
            vk::Extent2D extents(width, height);

            if (extents != m_renderer.getSwapchainExtents()) {
                m_mainApp->regenerateSwapchainResources(extents);
            }
        }

        m_mainApp->windowResized(width, height);

        return true;
    }

    void Window::run() {
        TimePoint thisFrame = std::chrono::high_resolution_clock::now();
        TimePoint lastFrame;

        while (!glfwWindowShouldClose(m_nativeWindow)) {
            glfwPollEvents();
            m_renderer.waitForNextRenderFrame();

            lastFrame = thisFrame;
            thisFrame = std::chrono::high_resolution_clock::now();
            TimeDelta delta = thisFrame - lastFrame;

            m_mainApp->simulateFrame(thisFrame, delta);

            if (!m_minimized) {
                m_mainApp->renderFrame(thisFrame, delta);
            }
        }
    }
}
