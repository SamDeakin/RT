#include "RT2/RT2App.hpp"

namespace RT2 {
    RT2::RT2App::RT2App(Core::Renderer& renderer, Core::V2AppBase::Parameters& parameters)
        : V2AppBase(renderer, parameters) {}

    RT2App::~RT2App() noexcept {}

    void RT2App::createDynamicRenderResources(const Core::V2AppBase::ResourceParameters& parameters) {}
    void RT2App::cleanupDynamicRenderResources() {}
    void RT2App::recordCommandBuffersInitial(std::vector<vk::CommandBuffer>& buffers) {}
    void RT2App::recordCommandBuffersPerFrame(std::vector<vk::CommandBuffer>& buffers) {}

    void RT2App::simulateFrame(Core::TimePoint now, Core::TimeDelta delta) {}
}
