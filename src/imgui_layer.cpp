#include <imgui_layer.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace RSCV
{
    void ImGuiLayer::Attach(const GLFWwindow *window)
    {
        // Configurando contexto do Dear ImGui 
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Habilita Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Habilita Docking

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(const_cast<GLFWwindow*>(window), true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    void ImGuiLayer::Dettach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::BeginDraw()
    {
        // Inicia o Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::EndDraw()
    {
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}