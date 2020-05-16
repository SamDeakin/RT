#pragma once

#include <Core/App.hpp>
#include <Core/DescriptorSetLayout.hpp>
#include <Core/PipelineLayout.hpp>
#include <Core/RenderPass.hpp>

#include <vk_mem_alloc.hpp>

#include <vector>

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
         * Perform a simulation frame
         * @param now: The high-resolution time right now
         * @param delta: The time since the last render frame
         */
        void simulateFrame(Core::TimePoint now, Core::TimeDelta delta) final;

        /**
         * Regenerate vulkan objects that change when the swapchain must be recreated
         * @param viewport: The new viewport extents
         */
        void regenerateSwapchainResources(vk::Extent2D viewport) final;

    private:
        Core::App::Parameters& m_runtimeParameters;
        Core::Renderer& m_renderer;
        vk::Device m_device;
        vma::Allocator m_allocator;

        std::unique_ptr<Core::RenderPass> m_basicRenderPass;
        std::unique_ptr<Core::DescriptorSetLayout> m_emptyDescriptorSetLayout;
        std::unique_ptr<Core::PipelineLayout> m_emptyPipelineLayout;
        std::unique_ptr<Core::GraphicsPipeline> m_simpleTrianglePipeline;

        /// Members recreated on swapchain recreation
        struct FramebufferData{
            vk::Image colourAttachment0Image;
            vma::Allocation colourAttachment0ImageAllocation;
            vk::ImageView colourAttachment0ImageView;

            vk::Framebuffer framebuffer;
        };
        std::vector<FramebufferData> m_framebufferData;

        vk::CommandPool m_renderCommandPool;
        vk::CommandPool m_transferCommandPool;
        const Core::QueueGroup& m_graphicsQueue;
        const Core::QueueGroup& m_transferQueue;
        const Core::QueueGroup& m_presentQueue;
        std::vector<vk::CommandBuffer> m_graphicsCommandBuffers;
        std::vector<vk::Fence> m_commandBufferFences; // To signal when it is safe to modify command buffers

        // Semaphores for render signalling
        vk::Semaphore m_swapchainImageSemaphore;
        vk::Semaphore m_renderCompletedSemaphore;
        vk::Semaphore m_copyCompletedSemaphore;

        // Renderable data
        vk::Buffer m_vertexBuffer;
        vma::Allocation m_vertexBufferAllocation;

        // -- Begin ctor helpers --

        void initRenderPass();
        void initPipeline();
        void initRenderData();
        void initSemaphores();
        void initCommandPools();

        // -- End ctor helpers --

        // -- Helpers for swapchain recreation --

        /// Create resources that are specific to each swapchain.
        /// Also used by the ctor
        void createSwapchainResources(int width, int height);

        /// Destroy only resources that are specific to each swapchain.
        void destroySwapchainResources();

        /// Create and record command buffers used for drawing
        void createCommandBuffers(int width, int height);

        /// Record into the current set of command buffers. Expects the command buffers to be ready for recording
        void recordCommandBuffers(int width, int height);

        /// Delete the current set of command buffers
        void destroyCommandBuffers();

        // -- End swapchain recreation helpers --
    };
}
