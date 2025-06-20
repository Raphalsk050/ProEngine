#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <entt.hpp>
#include "glad/glad.h"
#include <utils/Entity.h>
#include <filament/LightManager.h>
#include <memory>

#include "Core/Types.h"
#include "Core/Camera/Camera3D.h"
#include "Core/Camera/FilamentCamera.h"
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
        Ref<Model> model;
        Ref<Material> OverrideMaterial;

        ModelRendererComponent() = default;
        ModelRendererComponent(const ModelRendererComponent&) = default;

        ModelRendererComponent(const Ref<Model>& m)
        {
            model = m;
        }
    };

    struct RendererComponent
    {
        MeshType mesh{MeshType::Triangle};
        glm::vec4 color{1.0f};
        bool depth_test{false};
        bool culling{false};
        GLenum cull_face{GL_BACK};
        bool double_sided{true};
        std::shared_ptr<Model> model{nullptr};
        std::shared_ptr<Mesh> mesh_ptr{nullptr};
    };

    struct InteractableComponent
    {
        bool interactable = true;
    };

    struct CameraComponent
    {
        Camera3D camera{};
        bool primary{true};
    };

    struct FilamentCameraComponent
    {
        FilamentCamera camera{};
        bool primary{true};
    };

    struct LightComponent {
        filament::LightManager::Type type{filament::LightManager::Type::POINT};
        glm::vec3 color{1.0f};
        float intensity{1.0f};
        utils::Entity entity{};
    };

    struct FilamentEntityComponent {
        utils::Entity entity{};
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
