#pragma once

#include <Core/InputReceiver.hpp>
#include <Core/RenderTypes.hpp>
#include <Core/Renderer.hpp>

namespace Core {
    class V1AppBase : public InputReceiver {
    public:
        /// A basic set of parameters used to run the app
        struct Parameters {
            int width;
            int height;
        };

        explicit V1AppBase(Renderer& renderer, Parameters& parameters);
        ~V1AppBase() override = default;

        /// Disallowed operations
        V1AppBase(V1AppBase& other) = delete;
        V1AppBase(V1AppBase&& other) = delete;
        V1AppBase& operator=(V1AppBase& other) = delete;
        V1AppBase& operator=(V1AppBase&& other) = delete;

        /**
         * Begin rendering the next frame
         * @param now: The high-resolution time right now
         * @param delta: The time since the last render frame
         */
        virtual void renderFrame(Core::TimePoint now, Core::TimeDelta delta) = 0;

        /**
         * Perform a simulation frame
         * @param now: The high-resolution time right now
         * @param delta: The time since the last render frame
         */
        virtual void simulateFrame(Core::TimePoint now, Core::TimeDelta delta) = 0;

        /**
         * Regenerate vulkan objects that change when the swapchain must be recreated
         * @param viewport: The new viewport extents
         */
        virtual void regenerateSwapchainResources(vk::Extent2D viewport) = 0;
    };
}
