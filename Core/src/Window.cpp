#include "Core/Window.hpp"
#include <iostream>
namespace Core {

    Window::Window(Renderer& renderer, int width, int height)
        : m_renderer(renderer) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_nativeWindow = glfwCreateWindow(width, height, "RT", nullptr, nullptr);
        fixTopBar(width, height);
    }

    Window::~Window() noexcept { glfwDestroyWindow(m_nativeWindow); }

    GLFWwindow* Window::getNativeWindow() { return m_nativeWindow; }

    void Window::fixTopBar(int requestedWidth, int requestedHeight) {
        int width, height;
        glfwGetWindowSize(m_nativeWindow, &width, &height);
        int left, top, right, bottom;
        glfwGetWindowFrameSize(m_nativeWindow, &left, &top, &right, &bottom);

        if (width == requestedWidth && height == requestedHeight)
            return;

        // Attempt to just set the window size first. This may stop some platforms from
        // taking space from the requested size for the top bar.
        glfwSetWindowSize(m_nativeWindow, requestedWidth, requestedHeight);

        glfwGetWindowSize(m_nativeWindow, &width, &height);
        if (width == requestedWidth && height == requestedHeight)
            return;

        glfwGetWindowFrameSize(m_nativeWindow, &left, &top, &right, &bottom);

        // Attemp to add the top bar (and other spacing) to the size when requesting
        glfwSetWindowSize(m_nativeWindow, left + width + right, top + height + bottom);

        glfwGetWindowSize(m_nativeWindow, &width, &height);
        if (width == requestedWidth && height == requestedHeight)
            return;

        // If that didn't work then just go back to setting the requested size
        glfwSetWindowSize(m_nativeWindow, requestedWidth, requestedHeight);
    }
}
