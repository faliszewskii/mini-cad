//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_WORKSPACEMODULE_H
#define OPENGL_SANDBOX_WORKSPACEMODULE_H

#include <glm/gtc/type_ptr.hpp>
#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "../../../logic/algebra/Rect.h"
#include "../../gui/workspaces/LogPanelModuleWorkspace.h"
#include "../../gui/workspaces/DebugOverlayModuleWorkspace.h"
#include "../../gui/workspaces/GizmoModuleWorkspace.h"
#include "../../gui/workspaces/GridModuleWorkspace.h"
#include "../../gui/workspaces/WireframeRenderModuleWorkspace.h"
#include "../../gui/workspaces/EntityListWorkspace.h"
#include "../../gui/workspaces/CursorWorkspace.h"

class WorkspaceModule {
    const int workspaceWidth;
    std::optional<std::reference_wrapper<const std::pair<std::string, std::function<void(AppState&)>>>> selectedCategory = {};
public:
    explicit WorkspaceModule(int workspaceWidth) : workspaceWidth(workspaceWidth) {}

    void listCategories() {
        const static std::vector<std::pair<std::string, std::function<void(AppState&)>>> categories {
                {"Entity List", EntityListWorkspace::render},
                {"Cursor", CursorWorkspace::render},
                {"Wireframe Render Module", WireframeRenderModuleWorkspace::render},
                {"Log Panel Module",        LogPanelModuleWorkspace::render},
                {"Debug Overlay Module",    DebugOverlayModuleWorkspace::render},
                {"Gizmo Module",            GizmoModuleWorkspace::render},
                {"Grid Module",             GridModuleWorkspace::render},
        };
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        if(ImGui::BeginChild("Categories#Workspace", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (auto &category : categories) {
                bool isSelected = selectedCategory && &selectedCategory->get() == &category;
                if (ImGui::Selectable(category.first.c_str(), isSelected)) {
                    selectedCategory = category;
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
    }

    void run(AppState &appState) {
        const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y));
        ImGui::SetNextWindowSize(ImVec2(workspaceWidth, main_viewport->WorkSize.y));
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
        if (!ImGui::Begin("Main View", nullptr, window_flags)) {
            ImGui::End();
            return;
        }
        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

        listCategories();
        if(selectedCategory) {
            if(ImGui::BeginChild("Workspace#Workspace", ImVec2(0, 0))) {
                ImGui::SeparatorText(selectedCategory.value().get().first.c_str());
                selectedCategory.value().get().second(appState);
                ImGui::EndChild();
            }
        } else ImGui::SeparatorText("Select Category");

        ImGui::PopItemWidth();
        ImGui::End();
    }

    static void render(AppState &appState)  {
    }
};

#endif //OPENGL_SANDBOX_WORKSPACEMODULE_H
