#include "RT1/RT1App.hpp"

#include <Core/Shader.hpp>

namespace RT1 {

    RT1App::RT1App(Core::Renderer& renderer)
        : m_renderer(renderer) {
        // TODO We just construct these to show that we CAN for now
        Core::Shader("Resources/Shaders/trivial.vert.spv", Core::ShaderType::eVertex, renderer.getDevice());
        Core::Shader("Resources/Shaders/xyzToRgb.frag.spv", Core::ShaderType::eFragment, renderer.getDevice());
    }

    RT1App::~RT1App() = default;

    void RT1App::renderFrame(Core::TimePoint now, Core::TimeDelta delta) {}
}
