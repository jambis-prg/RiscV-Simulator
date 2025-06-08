#pragma once

#include <stdint.h>
#include <GLFW/glfw3.h>

#if GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR == 4
    #define RSCV_OWN_ALLOCATOR
    #include <glfw_custom_allocator.h>
#endif

namespace RSCV
{
    class Application
    {
    private:
        inline static bool s_GLFWInitialized = false;    
        inline static GLFWwindow* s_HWindow = nullptr;

        static void GLFWErrorCallback(int32_t code, const char* desc);
        static void GLFWFramebufferResizeCallback(GLFWwindow* window, int32_t width, int32_t height);

    public: 
        static bool Init();
        static void Shutdown();

        static void Run();
    };
}