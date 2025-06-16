#pragma once

#include "Config.h"
#include "Core/Renderer/Texture.h"
#include "vec4.hpp"

namespace ProEngine {

    class Material {
    public:
        Material() = default;
        virtual ~Material() = default;

        // Albedo (diffuse color)
        inline void SetAlbedoColor(const glm::vec4& color) { m_AlbedoColor = color; }
        inline const glm::vec4& GetAlbedoColor() const { return m_AlbedoColor; }

        // Textures
        inline void SetAlbedoMap(const Ref<Texture2D>& texture) { m_AlbedoMap = texture; }
        inline Ref<Texture2D> GetAlbedoMap() const { return m_AlbedoMap; }

        inline void SetNormalMap(const Ref<Texture2D>& texture) { m_NormalMap = texture; }
        inline Ref<Texture2D> GetNormalMap() const { return m_NormalMap; }

        inline void SetMetallicMap(const Ref<Texture2D>& texture) { m_MetallicMap = texture; }
        inline Ref<Texture2D> GetMetallicMap() const { return m_MetallicMap; }

        inline void SetRoughnessMap(const Ref<Texture2D>& texture) { m_RoughnessMap = texture; }
        inline Ref<Texture2D> GetRoughnessMap() const { return m_RoughnessMap; }

        // PBR parameters
        inline void SetMetallic(float metallic) { m_Metallic = metallic; }
        inline float GetMetallic() const { return m_Metallic; }

        inline void SetRoughness(float roughness) { m_Roughness = roughness; }
        inline float GetRoughness() const { return m_Roughness; }

    private:
        glm::vec4 m_AlbedoColor = { 1.0f, 1.0f, 1.0f, 1.0f };

        Ref<Texture2D> m_AlbedoMap;
        Ref<Texture2D> m_NormalMap;
        Ref<Texture2D> m_MetallicMap;
        Ref<Texture2D> m_RoughnessMap;

        float m_Metallic = 0.0f;
        float m_Roughness = 0.5f;
    };

}  // namespace BEngine