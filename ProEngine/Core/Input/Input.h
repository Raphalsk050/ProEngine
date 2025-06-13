#pragma once


#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseCodes.h"
#include <glm.hpp>

namespace ProEngine
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsKeyReleased(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);

        static glm::vec2 GetMousePosition();

        static float GetMouseX();

        static float GetMouseY();
    };
}
