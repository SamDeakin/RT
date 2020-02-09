#pragma once

#include "Core/Shader.hpp"

#include <vulkan/vulkan.hpp>

namespace Core {

    /**
     * A basic pipeline base class meant to support many different types of pipeline implementations
     * (eg. triangle, mesh, ray tracing, compute)
     */
    class PipelineBuilder {
    public:
        PipelineBuilder();
        virtual ~PipelineBuilder();

        /// -- Members for configuration --

        /**
         * Add a shader to this pipeline. Due to memory management reasons,
         * This object should be destroyed before any shader added using this method.
         * @param shader: The shader to add
         */
        void addShader(Shader& shader);

        /**
         * Set the pipeline to derive from to a previously created pipeline
         * Only one of pipeline and index can be set for derivation.
         * @param pipeline: The pipeline to derive from
         */
        void setBasePipeline(vk::Pipeline pipeline);

        /**
         * Set the pipeline to derive from to a pipeline that will be created at the same time as this pipeline.
         * Only one of pipeline and index can be set for derivation.
         * Note, the caller must keep the order passed to creation the same as described here.
         * @param index: The index of pipeline to derive from
         */
        void setBasePipeline(int32_t index);

        /// -- End members for configuration --

    protected:
        /// Handlers for pipeline implementations to support specific shader types
        virtual void addVertexShader(Shader& shader);
        virtual void addFragmentShader(Shader& shader);

        /// Flags used for creating the pipeline
        vk::PipelineCreateFlags m_pipelineCreateFlags = vk::PipelineCreateFlags();

        /// Members used for creating pipeline derivatives
        vk::Pipeline m_basePipeline = vk::Pipeline(); // Null handle
        int32_t m_basePipelineIndex = -1;
    };
}
