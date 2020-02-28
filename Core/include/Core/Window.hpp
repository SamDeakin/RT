#pragma once

#include <Core/App.hpp>
#include <Core/Renderer.hpp>
#include <Core/RenderTypes.hpp>

#include <GLFW/glfw3.h>

#include <memory>

namespace Core {

    class Window : public InputReceiver {
    public:
        explicit Window(int width, int height);
        virtual ~Window() noexcept;

        GLFWwindow* getNativeWindow();

        /**
         * Set the application that will be run and receive events
         * @param app: The implementation of Core::App
         */
        void setApp(App* app);

        bool windowResized(int width, int height) override;

    private:
        GLFWwindow* m_nativeWindow;

        /// A helper for the ctor: corrects the window height to be the requested height.
        /// Useful for cases where the native height differs slightly due to the top bar or other.
        void fixTopBar(int requestedWidth, int requestedHeight);

        /// A ctor helper to register all callbacks that will be used during runtime
        void registerCallbacks();

        bool m_minimized;

        std::unique_ptr<App> m_mainApp;
    };
}
