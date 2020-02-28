#pragma once

#include <Core/App.hpp>
#include <Core/DescriptorSetLayout.hpp>
#include <Core/PipelineLayout.hpp>
#include <Core/RenderPass.hpp>

namespace RT1 {

    class RT1App : public Core::App {
    public:
        /**
         * @param renderer: The renderer to use for graphics
         */
        explicit RT1App(Core::Renderer& renderer, Core::App::Parameters& parameters);
        ~RT1App() noexcept final;

    protected:
        /**
         * Begin rendering the next frame
         * @param now: The high-resolution time right now
         * @param delta: The time since the last render frame
         */
        void renderFrame(Core::TimePoint now, Core::TimeDelta delta) final;

        /**
         * Regenerate vulkan objects that change when the swapchain must be recreated
         * @param viewport: The new viewport extents
         */
        void regenerateSwapchainResources(vk::Extent2D viewport) final;

    private:
        Core::App::Parameters& m_runtimeParameters;
        Core::Renderer& m_renderer;
        vk::Device& m_device;

        std::unique_ptr<Core::RenderPass> m_basicRenderPass;
        std::unique_ptr<Core::DescriptorSetLayout> m_emptyDescriptorSetLayout;
        std::unique_ptr<Core::PipelineLayout> m_emptyPipelineLayout;
        std::unique_ptr<Core::GraphicsPipeline> m_simpleTrianglePipeline;

        /// Members recreated on swapchain recreation

        // -- Begin ctor helpers --

        void initRenderPass();
        void initPipeline();

        // -- End ctor helpers --

        // -- Helpers for swapchain recreation --

        /// Create resources that are specific to each swapchain.
        /// Also used by the ctor
        void createSwapchainResources(int width, int height);

        /// Destroy only resources that are specific to each swapchain.
        void destroySwapchainResources();

        // -- End swapchain recreation helpers --
    };
}
