#pragma once

#include <glm/glm.hpp>

namespace Core {

    enum class QueueType {
        Graphics,
        Transfer,
        Compute,
        SparseBinding,
        Present,
    };

}