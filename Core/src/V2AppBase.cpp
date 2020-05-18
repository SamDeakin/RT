#include "Core/V2AppBase.hpp"

namespace Core {
    V2AppBase::V2AppBase(Renderer& renderer, Parameters& parameters)
        : V1AppBase(renderer, parameters)
        , m_renderer(renderer) {}

    // Shutdown should be called before destruction to avoid leaks.
    V2AppBase::~V2AppBase() noexcept = default;

    void V2AppBase::renderFrame(Core::TimePoint now, Core::TimeDelta delta) {
        // TODO Render frame
    }

    void V2AppBase::regenerateSwapchainResources(vk::Extent2D viewport) {
        cleanupDynamicRenderResources();
        cleanupSwapchainResources();
        m_renderer.recreateSwapChain(viewport);

        ResourceParameters parameters{
            viewport,
        };
        createSwapchainResources(parameters);
        createDynamicRenderResources(parameters);
    }

    void V2AppBase::startup(const ResourceParameters &parameters) {
        createSwapchainResources(parameters);
        createDynamicRenderResources(parameters);
    }

    void V2AppBase::shutdown() {
        cleanupDynamicRenderResources();
        cleanupSwapchainResources();
    }

    void V2AppBase::createSwapchainResources(const V2AppBase::ResourceParameters& parameters) {
        // TODO Init resources
    }

    void V2AppBase::cleanupSwapchainResources() {
        // TODO Cleanup resources
    }
}
