#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <entt.hpp>
#include "glad/glad.h"
#include <memory>
#include <utility>

#include "Core/Types.h"
#include "Core/Camera/Camera3D.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/Mesh.h"

namespace ProEngine
{
    struct ComponentBase
    {
        std::string name = "Component";
        uint id = 0;

        explicit ComponentBase(std::string name = "Component", const uint id = 0) : name(std::move(name)), id(id)
        {
        }
    };

    struct TagComponent : public ComponentBase
    {
        std::string tag{"Entity"};

        explicit TagComponent(std::string tag) : ComponentBase("Tag"), tag(std::move(tag))
        {
        }
    };

    struct TransformComponent : public ComponentBase
    {
        TransformComponent() : ComponentBase("Transform") {}

        glm::vec3 position{0.0f};
        glm::vec3 rotation{0.0f};
        glm::vec3 scale{1.0f};

        entt::entity parent{entt::null};
        entt::entity first_child{entt::null};
        entt::entity next_sibling{entt::null};
        entt::entity prev_sibling{entt::null};

        [[nodiscard]] glm::mat4 LocalMatrix() const
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

    struct ModelRendererComponent : public ComponentBase
    {
        Ref<Model> model;
        Ref<Material> OverrideMaterial;

        ModelRendererComponent() : ComponentBase("ModelRenderer") {}
        ModelRendererComponent(const ModelRendererComponent&) = default;

        explicit ModelRendererComponent(const Ref<Model>& m)
        {
            model = m;
        }
    };

    struct RendererComponent : public ComponentBase
    {
        RendererComponent() : ComponentBase("Renderer") {}
        MeshType mesh{MeshType::Triangle};
        glm::vec4 color{1.0f};
        bool depth_test{false};
        bool culling{false};
        GLenum cull_face{GL_BACK};
        bool double_sided{true};
        std::shared_ptr<Model> model{nullptr};
        std::shared_ptr<Mesh> mesh_ptr{nullptr};
    };

    struct InteractableComponent : public ComponentBase
    {
        InteractableComponent() : ComponentBase("Interactable") {}
        bool interactable = true;
    };

    struct CameraComponent : public ComponentBase
    {
        CameraComponent() : ComponentBase("Camera") {}
        Camera3D camera{};
        bool primary{true};
    };

    enum class LightType
    {
        Directional = 0,
        Point = 1,
        Spot = 2
    };

    struct LightComponent : public ComponentBase
    {
        LightComponent () : ComponentBase("Light") {}
        LightType type{LightType::Point};
        glm::vec3 color{1.0f};
        float intensity{1.0f};
        glm::vec3 direction{0.0f, -1.0f, 0.0f};
        float range{10.0f};
        float inner_cutoff{12.5f};
        float outer_cutoff{17.5f};
    };

    struct CameraBoomComponent : public ComponentBase
    {
        CameraBoomComponent () : ComponentBase("CameraBoom") {}
        float arm_length{5.0f};
        bool collision_test{true};
        entt::entity target{entt::null};
    };

    enum class CameraType
    {
        FirstPerson = 0,
        ThirdPerson = 1
    };

    struct CharacterControllerComponent : public ComponentBase
    {
        CharacterControllerComponent() : ComponentBase("CharacterController") {}
        CameraType camera_type{CameraType::FirstPerson};
        float move_speed{5.0f};
        float jump_force{1.0f};
    };
} // namespace ProEngine
