//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_CAMERASWORKSPACE_H
#define OPENGL_SANDBOX_CAMERASWORKSPACE_H

#include "../../../../logic/state/AppState.h"
#include "imgui.h"

namespace CamerasWorkspace {
    inline void render(AppState &appState) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        if(ImGui::BeginChild("Cameras#Workspace", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (auto &camera : appState.cameras) {
                if (ImGui::Selectable(camera.getName().c_str(), appState.selectionGroup.getSelectedCamera().has_value())) {
                    appState.selectionGroup.setFocus(camera);
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();

        // TODO Camera UI
    }
};

#endif //OPENGL_SANDBOX_CAMERASWORKSPACE_H
