#include "RT1/RT1Window.hpp"

#include <Core/Shader.hpp>

namespace RT1 {

    RT1Window::RT1Window(Core::Renderer& renderer)
        : Window(renderer, 1920, 1080) {
        // TODO We just construct these to show that we CAN for now
        Core::Shader("Resources/Shaders/trivial.vert.spv", Core::ShaderType::eVertex, renderer.getDevice());
        Core::Shader("Resources/Shaders/xyzToRgb.frag.spv", Core::ShaderType::eFragment, renderer.getDevice());
    }

    RT1Window::~RT1Window() noexcept {}
}
