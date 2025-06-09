#include <imgui_layer.h>

#define IMGUI_IMPL_HAS_DOCK
#define IMGUI_HAS_DOCK   // (opcional e não oficial, só para clareza)
#define IMGUI_ENABLE_VIEWPORTS

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
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Habilita Docking

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

    void ImGuiLayer::Docking()
    {
        // Tudo isso foi retirado do exemplo de docking do ImGui
        // Nem eu sei ao certo oq tá acontecendo aqui
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        // Setando o docking para ter o mesmo tamanho, posição e a mesma viewport 
        // Da viewport da janela principal
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Setando para o docking não ter arrendondamento nem bordas
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        // Setando flags para o docking não ter barra de título, não colapsar, não ser redimensionável, não se mover
        // Não pegar o foco
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // Retira o padding do docking

        ImGui::Begin("DockSpace", NULL, windowFlags); // Cria a janela de docking

        // Popa os estilos que foram impregados (Arrendondamento = 0, Borda = 0, Padding = 0)
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        //	-- Submit the DockSpace --
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }

        ImGui::End();
    }
}