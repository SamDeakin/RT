#pragma once

#include <Core/Window.hpp>

namespace RT1 {

    class RT1Window : public Core::Window {
    public:
        explicit RT1Window(Core::Renderer& renderer);
        ~RT1Window() noexcept final;
    };
}
