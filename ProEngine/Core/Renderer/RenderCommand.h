#pragma once
#include "PEPCH.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/RenderCommand.h"

namespace ProEngine
{
    // TODO: implement
    class VertexArray;

    class RenderCommand
    {
    public:
        static void Init()
        {
            renderer_api_->Init();
        }

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            renderer_api_->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color)
        {
            renderer_api_->SetClearColor(color);
        }

        static void Clear()
        {
            renderer_api_->Clear();
        }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            //renderer_api_->DrawIndexed(vertexArray, indexCount);
        }

        static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
        {
            //renderer_api_->DrawLines(vertexArray, vertexCount);
        }

        static void SetLineWidth(float width)
        {
            renderer_api_->SetLineWidth(width);
        }

    private:
        static Scope<RendererAPI> renderer_api_;
    };
}
