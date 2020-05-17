#pragma once

#include <Core/RenderTypes.hpp>
#include <Core/Renderer.hpp>
#include <Core/V1AppBase.hpp>

#include <GLFW/glfw3.h>

#include <memory>

namespace Core {

    class Window : protected InputReceiver {
    public:
        explicit Window(Renderer& renderer, int width, int height);
        ~Window() noexcept override;

        GLFWwindow* getNativeWindow();

        /**
         * Set the application that will be run and receive events
         * @param app: The implementation of Core::App
         */
        void setApp(V1AppBase* app);

        /**
         * Get the size of the viewport currently
         * @return The size as received from the window api
         */
        vk::Extent2D getViewportExtents();

        /**
         * A simple single-threaded run-loop
         */
        virtual void run();

        /// Event handling overrides, only visible to subclasses
        bool windowResized(int width, int height) override;

    private:
        GLFWwindow* m_nativeWindow;
        Renderer& m_renderer;

        /// A helper for the ctor: corrects the window height to be the requested height.
        /// Useful for cases where the native height differs slightly due to the top bar or other.
        void fixTopBar(int requestedWidth, int requestedHeight);

        /// A ctor helper to register all callbacks that will be used during runtime
        void registerCallbacks();

        bool m_minimized;

        std::unique_ptr<V1AppBase> m_mainApp;
    };
}
