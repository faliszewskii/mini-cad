//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_MENUBARMODULE_H
#define OPENGL_SANDBOX_MENUBARMODULE_H

#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "nfd.h"

class MenuBarModule {
public:
    explicit MenuBarModule() {}

    void run(AppState &appState) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}
                if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}
                if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
                ImGui::Separator();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

};

#endif //OPENGL_SANDBOX_MENUBARMODULE_H
