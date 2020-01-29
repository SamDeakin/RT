#include "Core/Window.hpp"

namespace Core {

    Window::Window(Renderer& renderer)
        : m_renderer(renderer) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_nativeWindow = glfwCreateWindow(1920, 1080, "RT", nullptr, nullptr);
    }

    Window::~Window() noexcept { glfwDestroyWindow(m_nativeWindow); }

    GLFWwindow* Window::getNativeWindow() { return m_nativeWindow; }
}
