#pragma once

#include <chrono>
#include "GLFW/glfw3.h"

namespace ProEngine
{
    class Time
    {
    public:
        Time()
        {
            Reset();
        }

        void Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        static float GetTime()
        {
            return glfwGetTime();
        }

        float Elapsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).
                count() * 0.001f * 0.001f * 0.001f;
        }

        float ElapsedMillis()
        {
            return Elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };
}
