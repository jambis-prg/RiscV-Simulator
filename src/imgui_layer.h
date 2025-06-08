#pragma once

#include <GLFW/glfw3.h>

namespace RSCV
{
    class ImGuiLayer
    {
    private:
        inline static bool s_Initialized = false;
    public:
        static void Attach(const GLFWwindow *window);
        static void Dettach();

        static void BeginDraw();
        static void EndDraw();
    };
}