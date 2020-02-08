#pragma once

#include <Core/Renderer.hpp>

#include <GLFW/glfw3.h>

namespace Core {

    class Window {
    public:
        explicit Window(Renderer& renderer, int width, int height);
        virtual ~Window() noexcept;

        GLFWwindow* getNativeWindow();

    protected:
        Renderer& m_renderer;
        GLFWwindow* m_nativeWindow;

    private:
        /// A helper for the ctor: corrects the window height to be the requested height.
        /// Useful for cases where the native height differs slightly due to the top bar or other.
        void fixTopBar(int requestedWidth, int requestedHeight);
    };
}
