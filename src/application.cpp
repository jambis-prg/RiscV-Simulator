#include <application.h>
#include <logger.h>

#include <imgui.h>

#include <GLFW/glfw3.h>
#include <imgui_layer.h>

namespace RSCV
{
    bool Application::Init()
    {   
#ifdef RSCV_OWN_ALLOCATOR
        GLFWallocator ownAllocator;
        ownAllocator.allocate = GLFWCustomAllocator::Allocate;
        ownAllocator.reallocate = GLFWCustomAllocator::Reallocate;
        ownAllocator.deallocate = GLFWCustomAllocator::Free;
        ownAllocator.user = nullptr;

        glfwInitAllocator(&ownAllocator);
#endif

        // Setando a função que será chamada quando ocorrer um erro no GLFW
        glfwSetErrorCallback(GLFWErrorCallback); 
        
        if (!glfwInit()) return false;

        s_GLFWInitialized = true; // Indica que o GLFW conseguiu inicializar
        
#ifndef NDEBUG
        // Obtendo a versão do GLFW e printando
        int32_t major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        RSCV_LOG_DEBUG("GLFW Version (%d.%d.%d)", major, minor, revision);
#endif
        
        // Criando janela da aplicação
        s_HWindow = glfwCreateWindow(640, 480, "Risc V Simulator", nullptr, nullptr);
        
        if (!s_HWindow) return false;
        
        // Setando a função que será chamada quando o framebuffer for redimensionado
        glfwSetFramebufferSizeCallback(s_HWindow, GLFWFramebufferResizeCallback); 
        glfwMakeContextCurrent(s_HWindow); // Criando o contexto de renderização para a janela
        glfwSwapInterval(1); // Ativando V-Sync

        ImGuiLayer::Attach(s_HWindow);

        return true;
    }
        
    void Application::Shutdown()
    {
        if (s_HWindow) 
        {
            ImGuiLayer::Dettach();
            glfwDestroyWindow(s_HWindow);
        }
        if (s_GLFWInitialized) glfwTerminate();

#if RSCV_OWN_ALLOCATOR
        RSCV_GLFW_DUMP_MEM();
#endif
    }

    void Application::Run()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Definindo a cor branca para limpar a tela

        while(!glfwWindowShouldClose(s_HWindow))
        {
            glfwPollEvents(); // Tratando eventos

            ImGuiLayer::BeginDraw();
            glClear(GL_COLOR_BUFFER_BIT); // Limpando tela
            
            ImGui::Begin("Minha Janela");
            ImGui::End();

            ImGuiLayer::EndDraw();
            
            glfwSwapBuffers(s_HWindow); // Trocando buffers
        }
    }

    void Application::GLFWErrorCallback(int32_t code, const char *desc)
    {
        RSCV_LOG_ERROR("%s - (%d)", code, desc);
    }

    void Application::GLFWFramebufferResizeCallback(GLFWwindow *window, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height); // Redimensiona o tamanho da viewport
    }
}