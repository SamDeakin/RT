#pragma once

#include <Core/Window.hpp>

namespace RT1 {

    class RT1Window : public Core::Window {
    public:
        /**
         * Performs most setup including loading all resources required.
         * @param renderer: The renderer to use for graphics
         */
        explicit RT1Window(Core::Renderer& renderer);
        ~RT1Window() noexcept final;
    };
}
