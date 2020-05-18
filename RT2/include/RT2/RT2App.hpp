#pragma once

#include <Core/V2AppBase.hpp>

namespace RT2 {
    class RT2App final : public Core::V2AppBase {
    public:
        explicit RT2App(Core::Renderer& renderer, Core::V2AppBase::Parameters& parameters);
        ~RT2App() noexcept final;

        /// Disallowed operations
        RT2App(RT2App& other) = delete;
        RT2App(RT2App&& other) = delete;
        RT2App& operator=(RT2App& other) = delete;
        RT2App& operator=(RT2App&& other) = delete;

        void simulateFrame(Core::TimePoint now, Core::TimeDelta delta) final;

    protected:
        void createDynamicRenderResources(const ResourceParameters& parameters) final;
        void cleanupDynamicRenderResources() final;
        void recordCommandBuffersInitial(std::vector<vk::CommandBuffer>& buffers) final;
        void recordCommandBuffersPerFrame(std::vector<vk::CommandBuffer>& buffers) final;
    };
}
