#include "menu/menu.h"
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"


void open_config_menu(GLFWwindow* window, EngineConfig* config) {

    glfwSetCursorPosCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetKeyCallback(window, nullptr);
    glfwSetCharCallback(window, nullptr);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().FontGlobalScale = 1.5f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize  |
        ImGuiWindowFlags_NoMove    |
        ImGuiWindowFlags_NoCollapse
    ;

    bool config_done = false;
    while (!config_done && !glfwWindowShouldClose(window)) {
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_Always);

        ImGui::Begin("Map Configuration", nullptr, flags);

        ImGui::InputInt("Radius", &config->map_radius);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::InputScalar("WFC Seed", ImGuiDataType_U32, &config->wfc_seed);
        ImGui::Spacing();
        ImGui::InputScalar("Height factors Seed", ImGuiDataType_U32, &config->hf_perlin_seed);
        ImGui::Spacing();
        ImGui::InputScalar("River Seed", ImGuiDataType_U32, &config->river_ridged_seed);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Real Time Render", &config->opengl_render);
        if (config->opengl_render) {
            ImGui::Spacing();
            ImGui::InputScalar("Render Speed", ImGuiDataType_U32, &config->opengl_step_counter);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        float button_w = 400, button_h = 40;
        float center_x_pos = (ImGui::GetWindowWidth() - button_w) * 0.5f;
        ImGui::SetCursorPosX(center_x_pos);
        if (ImGui::Button("Generate", ImVec2(button_w, button_h))) {
            config_done = true;
        }
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, 1000, 1000);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
