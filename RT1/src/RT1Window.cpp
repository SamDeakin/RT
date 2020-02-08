#include "RT1/RT1Window.hpp"

namespace RT1 {

    RT1Window::RT1Window(Core::Renderer& renderer)
        : Window(renderer, 1920, 1080) {}

    RT1Window::~RT1Window() noexcept {}
}
