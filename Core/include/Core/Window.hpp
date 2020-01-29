#pragma once

#include <Core/Renderer.hpp>

#include <GLFW/glfw3.h>

namespace Core {

    class Window {
    public:
        explicit Window(Renderer& renderer);
        virtual ~Window() noexcept;

        GLFWwindow* getNativeWindow();

    protected:
        Renderer& m_renderer;
        GLFWwindow* m_nativeWindow;
    };
}
