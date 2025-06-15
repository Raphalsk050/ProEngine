#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <entt.hpp>
#include "glad/glad.h"
#include <memory>

#include "Core/Renderer/Material.h"
#include "Core/Renderer/Mesh.h"

namespace ProEngine
{
    struct TagComponent
    {
        std::string tag{"Entity"};
    };

    struct TransformComponent
    {
        glm::vec3 position{0.0f};
        glm::vec3 rotation{0.0f};
        glm::vec3 scale{1.0f};

        entt::entity parent{entt::null};
        entt::entity first_child{entt::null};
        entt::entity next_sibling{entt::null};
        entt::entity prev_sibling{entt::null};

        glm::mat4 LocalMatrix() const
        {
            glm::mat4 mat(1.0f);
            mat = glm::translate(mat, position);
            mat = glm::rotate(mat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            mat = glm::rotate(mat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            mat = glm::rotate(mat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            mat = glm::scale(mat, scale);
            return mat;
        }
    };

    struct ModelRendererComponent
    {
        Ref<Model> Model;
        Ref<Material> OverrideMaterial;

        ModelRendererComponent() = default;
        ModelRendererComponent(const ModelRendererComponent&) = default;

        ModelRendererComponent(const Ref<ProEngine::Model>& model)
            : Model(model)
        {
        }
    };
} // namespace ProEngine
