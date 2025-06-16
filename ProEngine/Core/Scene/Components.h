#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <entt.hpp>
#include "glad/glad.h"
#include <memory>

#include "Core/Types.h"
#include "Core/Camera/Camera3D.h"
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

    struct RendererComponent
    {
        MeshType mesh{MeshType::Triangle};
        glm::vec4 color{1.0f};
        bool depth_test{true};
        bool culling{true};
        GLenum cull_face{GL_BACK};
        bool double_sided{false};
        std::shared_ptr<Model> model{nullptr};
        std::shared_ptr<Mesh> mesh_ptr{nullptr};
    };

    struct CameraComponent
    {
        Camera3D camera{};
        bool primary{true};
    };

    enum class LightType {
        Directional = 0,
        Point = 1,
        Spot = 2
    };

    struct LightComponent {
        LightType type{LightType::Point};
        glm::vec3 color{1.0f};
        float intensity{1.0f};
        glm::vec3 direction{0.0f, -1.0f, 0.0f};
        float range{10.0f};
        float inner_cutoff{12.5f};
        float outer_cutoff{17.5f};
    };

    struct CameraBoomComponent {
        float arm_length{5.0f};
        bool collision_test{true};
        entt::entity target{entt::null};
    };

    enum class CameraType {
        FirstPerson = 0,
        ThirdPerson = 1
    };

    struct CharacterControllerComponent {
        CameraType camera_type{CameraType::FirstPerson};
        float move_speed{5.0f};
        float jump_force{1.0f};
    };
} // namespace ProEngine
