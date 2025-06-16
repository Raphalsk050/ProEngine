#pragma once
#include "Core/Renderer/Framebuffer.h"

namespace ProEngine
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual uint32_t GetColorAttachmentRendererID() const override { return color_attachment_; }

    private:
        void Invalidate();

        uint32_t renderer_id_ = -1;
        uint32_t color_attachment_ = -1;
        uint32_t depth_attachment_ = -1;
    };
}
