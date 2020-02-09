#pragma once

#include "RT1/RT1App.hpp"

#include <Core/Window.hpp>

namespace RT1 {

    class RT1Window : public Core::Window {
    public:
        /**
         * @param renderer: The renderer to use for graphics
         */
        explicit RT1Window(Core::Renderer& renderer);
        ~RT1Window() noexcept final;

        void runApp(RT1App& app);
        void stopApp();
    };
}
