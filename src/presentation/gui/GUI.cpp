//
// Created by faliszewskii on 30.12.23.
//

#include <nfd.h>
#include <glm/gtc/type_ptr.hpp>
#include "GUI.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

GUI::GUI(GLFWwindow *window) {
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
}

void GUI::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void GUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}