#pragma once

#include <Core/RenderTypes.hpp>
#include <Core/Renderer.hpp>
#include <Core/V1AppBase.hpp>

#include <GLFW/glfw3.h>

#include <memory>

namespace Core {

    class V1WindowBase : protected InputReceiver {
    public:
        explicit V1WindowBase(Renderer& renderer, int width, int height);
        ~V1WindowBase() noexcept override;

        /**
         * Get the native GLFW window intended for rendering use with vulkan.
         * @return A pointer to the window.
         */
        GLFWwindow* getNativeWindow();

        /**
         * Set the application that will be run and receive events
         * @param app: The implementation of Core::App
         */
        void setApp(const std::shared_ptr<V1AppBase>& app);

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

    protected:
        Renderer& m_renderer;

    private:
        GLFWwindow* m_nativeWindow;

        /// A helper for the ctor: corrects the window height to be the requested height.
        /// Useful for cases where the native height differs slightly due to the top bar or other.
        void fixTopBar(int requestedWidth, int requestedHeight);

        /// A ctor helper to register all callbacks that will be used during runtime
        void registerCallbacks();

        bool m_minimized;

        std::shared_ptr<V1AppBase> m_mainApp;
    };
}
