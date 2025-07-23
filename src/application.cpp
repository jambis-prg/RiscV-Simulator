#include <application.h>
#include <logger.h>
#include <stdio.h>

#include <imgui.h>

#include <GLFW/glfw3.h>
#include <imgui_layer.h>

#include <ImGuiFileDialog.h>

#include "CPU/cpu.h"
#include "CPU/pipeline.h"
#include "CPU/single_cycle.h"


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
        char buffer[4096];

        CPU::SingleCycle singleCycle;
        CPU::CPU cpu(singleCycle);
        std::array<uint8_t, 1024> ram = { 
            0x93, // addi x1 x0 2
            0x00,
            0x20,
            0x00, 
            0x13, // addi x2 x0 3
            0x01,
            0x30,
            0x00,
            0xb3, // add x3 x1 x2 
            0x81,
            0x20,
            0x00,
            0x13, // addi x10 x0 1
            0x05,
            0x10,
            0x00,
            0x93, // addi x11 x0 42
            0x05,
            0xa0,
            0x02,
        };
        
        uint32_t selectedFlag = 0;
        while(!glfwWindowShouldClose(s_HWindow))
        {
            glfwPollEvents(); // Tratando eventos

            ImGuiLayer::BeginDraw();
            glClear(GL_COLOR_BUFFER_BIT); // Limpando tela
            
            ImGuiLayer::Docking(); // Tudo abaixo dessa linha poderá ser dockado

            ImGui::Begin("Memoria", nullptr);

            // Começar tabela
            if (ImGui::BeginTable("MemDump", 17, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
            {
                ImGui::TableSetupColumn("End");
                for (uint8_t j = 0; j < 16; j++)
                {
                    char tmpBuffer[20];
                    snprintf(tmpBuffer, sizeof(tmpBuffer), "0x%x", j);
                    ImGui::TableSetupColumn(tmpBuffer);
                }

                ImGui::TableHeadersRow();

                ImGuiListClipper clipper;
                clipper.Begin(ram.size() / 16);  // total de linhas da tabela

                while (clipper.Step()) 
                {
                    for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) 
                    {
                        ImGui::TableNextRow();
        
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("0x%x", row * 16);
        
                        for (uint8_t j = 0; j < 16; j++)
                        {
                            ImGui::TableSetColumnIndex(j + 1);
        
                            ImGui::Text("%x", ram[row * 16 + j]);
                        }
                    }
                }

                ImGui::EndTable();
            }

            ImGui::End();

            ImGui::Begin("Registradores", nullptr, ImGuiWindowFlags_MenuBar);
            // Começar tabela
            if (ImGui::BeginTable("RegsTable", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
            {
                ImGui::TableSetupColumn("Registrador");
                ImGui::TableSetupColumn("Valor");
                ImGui::TableHeadersRow();
                auto& regs = cpu.GetRegisters();
                for (uint8_t i = 0; i < regs.size(); i++)
                {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("x%d", i);

                    ImGui::TableSetColumnIndex(1);

                    ImGui::Text((selectedFlag >> i) & 1 ? "%d" : "0x%X", regs[i]);

                    // Detecta clique na linha inteira
                    ImGui::TableSetColumnIndex(0);
                    ImGuiID row_id = ImGui::GetID((void*)(intptr_t)i);
                    ImGui::PushID(i);
                    if (ImGui::Selectable("##row", false, ImGuiSelectableFlags_SpanAllColumns))
                    {
                        selectedFlag ^= 1 << i;
                    }
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }

            if(ImGui::BeginTable("Registradores de Controle", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
            {
                ImGui::TableSetupColumn("Registrador");
                ImGui::TableSetupColumn("Valor");
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("PC");

                ImGui::TableSetColumnIndex(1);

                ImGui::Text("0x%X", cpu.GetPC());

                ImGui::EndTable();
            }

            ImGui::End();

            // display
            ImVec2 maxSize = ImGui::GetWindowViewport()->WorkSize;  // The full display area
            ImVec2 minSize = ImVec2(maxSize.x * 0.5f, maxSize.y* 0.5f);  // Half the display area
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) 
            {
                if (ImGuiFileDialog::Instance()->IsOk()) 
                {
                    const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

                    RSCV_LOG_DEBUG("File choosed: %s", filePathName.c_str());
                    
                    std::ifstream fin(filePathName.c_str());
                    if (fin)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        fin.read(buffer, sizeof(buffer));
                        fin.close();
                    }
                }
                
                // close
                ImGuiFileDialog::Instance()->Close();
            }

            ImGui::Begin("Code", nullptr, ImGuiWindowFlags_NoCollapse);
            
            // open Dialog Simple
            if (ImGui::Button("Open File Dialog")) 
            {
                IGFD::FileDialogConfig config;
                config.path = "./InstructionsTest";
                config.flags = ImGuiFileDialogFlags_Modal;
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt", config);
            }

            if (ImGui::Button("Cycle"))
            {
                cpu.Cycle(ram);
                uint8_t i = 0;
                for (const auto& reg : cpu.GetRegisters())
                    RSCV_LOG_DEBUG("x%d: %x", i++, reg);
            }

            if (ImGui::Button("Reset"))
                cpu.Reset();

            // InputTextMultiline
            ImGui::SameLine();
            ImGui::BeginChild("TextArea", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::InputTextMultiline("##Code Text", buffer, sizeof(buffer), ImGui::GetContentRegionAvail());
            ImGui::EndChild();
            ImGui::End();

            ImGuiLayer::EndDraw();

            glfwSwapBuffers(s_HWindow); // Trocando buffers
        }
    }

    void Application::GLFWErrorCallback(int32_t code, const char *desc)
    {
        RSCV_LOG_ERROR("%s - (%d)", desc, code);
    }

    void Application::GLFWFramebufferResizeCallback(GLFWwindow *window, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height); // Redimensiona o tamanho da viewport
    }
}