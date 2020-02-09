#pragma once

#include <Core/GraphicsPipeline.hpp>
#include <Core/RenderTypes.hpp>
#include <Core/Renderer.hpp>

#include <vulkan/vulkan.hpp>

#include <chrono>
#include <memory>

namespace RT1 {
    class RT1App {
    public:
        explicit RT1App(Core::Renderer& renderer);
        ~RT1App();

        /**
         * Begin rendering the next frame
         * @param now: The high-resolution time right now
         * @param delta: The time since the last render frame
         */
        void renderFrame(Core::TimePoint now, Core::TimeDelta delta);

    private:
        Core::Renderer& m_renderer;
        vk::Device& m_device;

        std::unique_ptr<Core::GraphicsPipeline> m_simpleTrianglePipeline;
    };
}
