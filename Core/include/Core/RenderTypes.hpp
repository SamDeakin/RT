#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

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
        QueueType type;
        uint32_t familyIndex;
        std::vector<vk::Queue> queues;
    };

    using ShaderType = vk::ShaderStageFlagBits;

}
