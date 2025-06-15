#pragma once
#include "Config.h"
#include "Core/Renderer/RendererAPI.h"

namespace ProEngine {

    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;

        const FramebufferSpecification& GetSpecification() const { return m_Specification; }

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    protected:
        FramebufferSpecification m_Specification;
    };

}
