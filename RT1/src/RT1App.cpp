#include "RT1/RT1App.hpp"

#include <Core/RenderPassBuilder.hpp>
#include <Core/Shader.hpp>
#include <Core/TrianglePipelineBuilder.hpp>

namespace {
    struct Vertex {
        float x, y, z, w;
    };

    Vertex screenSpaceTriangle[] = {
        {-1, 1, 0.5, 1.0},
        {0, -1, 0.5, 1.0},
        {1, 1, 0.5, 1.0},
    };
}

namespace RT1 {

    RT1App::RT1App(Core::Renderer& renderer, Core::App::Parameters& parameters)
        : Core::App(renderer, parameters)
        , m_runtimeParameters(parameters)
        , m_renderer(renderer)
        , m_device(renderer.getDevice()) {
        initRenderPass();
        initPipeline();

        vk::Extent2D windowSize = m_renderer.getSwapchainExtents();
        createSwapchainResources(windowSize.width, windowSize.height);
    }

    RT1App::~RT1App() noexcept = default;

    void RT1App::initRenderPass() {
        Core::RenderPassBuilder builder;

        builder.addAttachment(vk::AttachmentDescription{
            vk::AttachmentDescriptionFlags(),
            m_renderer.getOutputFormat(),
            VULKAN_HPP_NAMESPACE::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferSrcOptimal,
        });

        vk::AttachmentReference colourInputAttachment{
            0,
            vk::ImageLayout::eColorAttachmentOptimal,
        };
        builder.addSubpass(vk::SubpassDescription{
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            0,
            nullptr,
            1,
            &colourInputAttachment,
            nullptr,
            nullptr,
            0,
            nullptr,
        });

        vk::RenderPassCreateInfo createInfo;
        builder.getRenderPassCreateInfo(createInfo);

        m_basicRenderPass = std::make_unique<Core::RenderPass>(m_device, createInfo);
    }

    void RT1App::initPipeline() {
        // Create a pipeline layout
        m_emptyDescriptorSetLayout = std::make_unique<Core::DescriptorSetLayout>(m_device, 0, nullptr);
        m_emptyPipelineLayout = std::make_unique<Core::PipelineLayout>(m_device, 1, &m_emptyDescriptorSetLayout->getHandle(), 0, nullptr);

        // Create a pipeline
        Core::TrianglePipelineBuilder pipelineBuilder;
        pipelineBuilder.setPipelineLayout(*m_emptyPipelineLayout);
        pipelineBuilder.setRenderPass(*m_basicRenderPass, 0);

        // There is only one colour attachment
        pipelineBuilder.addColourAttachmentBlendState();

        Core::Shader vertShader("Resources/Shaders/trivial.vert.spv", Core::ShaderType::eVertex, m_renderer.getDevice());
        pipelineBuilder.addShader(vertShader);
        Core::Shader fragShader("Resources/Shaders/xyzToRgb.frag.spv", Core::ShaderType::eFragment, m_renderer.getDevice());
        pipelineBuilder.addShader(fragShader);

        vk::Extent2D windowSize = m_renderer.getSwapchainExtents();
        pipelineBuilder.setWindowSize(windowSize.width, windowSize.height);

        pipelineBuilder.addVertexInputBindingDesc(vk::VertexInputBindingDescription{
            0,
            sizeof(Vertex),
            vk::VertexInputRate::eVertex,
        });
        pipelineBuilder.addVertexInputAttributeDesc(vk::VertexInputAttributeDescription{
            0,
            0,
            vk::Format::eR32G32B32A32Sfloat,
            0,
        });

        vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
        pipelineBuilder.getPipelineCreateInfo(pipelineCreateInfo);

        std::vector<std::unique_ptr<Core::GraphicsPipeline>> createdPipelines = m_renderer.createGraphicsPipelines(1, &pipelineCreateInfo);

        m_simpleTrianglePipeline = std::move(createdPipelines[0]);
    }

    void RT1App::createSwapchainResources(int width, int height) {
        // In general we only recreate resources that directly depend on the swapchain itself or the viewport size.
        // For full correctness, we would recreate based on the image format as well, but we only support one format
        // at time of writing and do not expect it to change during runtime.
    }

    void RT1App::destroySwapchainResources() {}

    void RT1App::regenerateSwapchainResources(vk::Extent2D viewport) {
        destroySwapchainResources();
        m_renderer.recreateSwapChain(viewport);
        createSwapchainResources(viewport.width, viewport.height);
    }

    void RT1App::renderFrame(Core::TimePoint now, Core::TimeDelta delta) {}
    void RT1App::simulateFrame(Core::TimePoint now, Core::TimeDelta delta) {}
}
