#include "Window.hpp"

namespace Core {
    Window::Window(Renderer& renderer)
        : m_renderer(renderer) {}
    Window::~Window() noexcept = default;
}
