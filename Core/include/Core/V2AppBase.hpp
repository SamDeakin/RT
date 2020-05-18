#pragma once

#include <Core/RenderTypes.hpp>
#include <Core/V1AppBase.hpp>

namespace Core {
    class V2AppBase : public V1AppBase {
    public:
        /// Parameters needed to run the app
        struct Parameters : public V1AppBase::Parameters {};

        /**
         *
         * @param renderer: A renderer that this app will use to get vulkan internal information
         * @param parameters: A list of runtime parameters used during construction
         */
        explicit V2AppBase(Renderer& renderer, Parameters& parameters);
        ~V2AppBase() noexcept override;

        struct ResourceParameters {
            vk::Extent2D viewport;
        };
        void startup(const ResourceParameters& parameters);
        void shutdown();

        virtual void recordCommandBuffersInitial(std::vector<vk::CommandBuffer>& buffers) = 0;
        virtual void recordCommandBuffersPerFrame(std::vector<vk::CommandBuffer>& buffers) = 0;

        /// Methods used to implement V2AppBase that should not be overridden by a consumer
//        void configure() final; // TODO To configure render resources needed by the base? Remove?
        void renderFrame(Core::TimePoint now, Core::TimeDelta delta) final;
        void regenerateSwapchainResources(vk::Extent2D viewport) final;

    protected:
        Renderer& m_renderer;

        /**
         * Derived classes should override this to receive notifications for when it should recreate
         * swapchain-dependent resources
         * @param parameters: parameters used for recreation
         */
        virtual void createDynamicRenderResources(const ResourceParameters& parameters) = 0;

        /**
         * Derived classes should implement this to receive notifications for when swapchain-dependent
         * resources should be released.
         */
        virtual void cleanupDynamicRenderResources() = 0;

    private:
        void createSwapchainResources(const ResourceParameters& parameters);
        void cleanupSwapchainResources();
    };
}
