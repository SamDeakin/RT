#pragma once

#include "Core/V1WindowBase.hpp"
#include "Core/V2AppBase.hpp"

#include <memory>

namespace Core {

    class V2WindowBase : public V1WindowBase {
    public:
        explicit V2WindowBase(Renderer& renderer, int width, int height);
        ~V2WindowBase() noexcept override;

        /**
         * Attach an application for rendering use. This app expects an application
         * build with V2 of the App api.
         * @param app: A valid V2 app, which will also be passed as a V1App to the V1WindowBase.
         */
        void setApp(const std::shared_ptr<V2AppBase>& app);

        /**
         * A run loop that is more complicated that V1WindowBase.
         * Improvements include calling to the attached app for dynamic resource creation and cleanup.
         */
        void run() override;

    private:
        std::shared_ptr<V2AppBase> m_mainApp;
    };
}
