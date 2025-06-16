#pragma once

#include "Core/Scene/Scene.h"
#include "Core/Scene/Components.h"
#include "Core/Renderer/Renderer3D.h"

namespace ProEngine {
class SceneRenderer {
public:
    static void RenderScene(Scene* scene);
};
}
