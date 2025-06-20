#include "Core/Camera/FilamentCamera.h"
#include "PEPCH.h"
#include <utils/EntityManager.h>
#include <gtc/matrix_transform.hpp>

namespace ProEngine {

FilamentCamera::FilamentCamera(filament::Engine* engine,
                               float fov, float aspectRatio,
                               float nearClip, float farClip)
    : engine_(engine) {
    entity_ = utils::EntityManager::get().create();
    camera_ = engine_->createCamera(entity_);
    SetPerspective(fov, aspectRatio, nearClip, farClip);
    SetPosition(position_);
}

FilamentCamera::~FilamentCamera() {
    if (engine_ && camera_) {
        engine_->destroyCameraComponent(entity_);
    }
}

void FilamentCamera::SetPerspective(float fov, float aspectRatio,
                                    float nearClip, float farClip) {
    m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    if (camera_) {
        camera_->setProjection(fov, aspectRatio, nearClip, farClip);
    }
}

void FilamentCamera::SetPosition(const glm::vec3& position) {
    position_ = position;
    if (camera_) {
        camera_->lookAt({position_.x, position_.y, position_.z},
                        {position_.x + forward_.x,
                         position_.y + forward_.y,
                         position_.z + forward_.z},
                        {0, 1, 0});
    }
}

void FilamentCamera::SetForwardDirection(const glm::vec3& direction) {
    forward_ = glm::normalize(direction);
    SetPosition(position_);
}

} // namespace ProEngine
