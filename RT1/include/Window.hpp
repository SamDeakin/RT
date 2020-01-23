#pragma once

#include "Renderer.hpp"

namespace Core {

    class Window {
    public:
        explicit Window(Renderer& renderer);
        virtual ~Window() noexcept;

    protected:
        Renderer& m_renderer;
    };
}