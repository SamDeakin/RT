#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include <chrono>
#include <vector>

namespace Core {

    enum class QueueType {
        Graphics,
        Transfer,
        Compute,
        SparseBinding,
        Present,
    };

    struct QueueGroup {
        QueueType primaryType;
        vk::QueueFlags supportedTypes;
        uint32_t familyIndex;
        std::vector<vk::Queue> queues;
    };

    using ShaderType = vk::ShaderStageFlagBits;

    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using TimeDelta = std::chrono::duration<double>;
}
