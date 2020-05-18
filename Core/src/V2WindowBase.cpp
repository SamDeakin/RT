#include "Core/V2WindowBase.hpp"

namespace Core {

    V2WindowBase::V2WindowBase(Renderer& renderer, int width, int height)
        : V1WindowBase(renderer, width, height) {}

    V2WindowBase::~V2WindowBase() noexcept = default;

    void V2WindowBase::setApp(const std::shared_ptr<V2AppBase>& app) {
        // TODO This is a poor solution for being able to reference the app as the latest version without casting
        // Something better would eliminate the type on the base or update the type on the base.
        // One extra pointer in memory per version# could be worse tho
        m_mainApp = app;
        V1WindowBase::setApp(app);
    }

    void V2WindowBase::run() {
        V2AppBase::ResourceParameters appParameters{
            m_renderer.getSwapchainExtents(),
        };
        m_mainApp->startup(appParameters);

        // TODO a real run loop

        m_mainApp->shutdown();
    }
}
