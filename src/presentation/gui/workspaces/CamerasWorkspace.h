//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_CAMERASWORKSPACE_H
#define OPENGL_SANDBOX_CAMERASWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "../../modules/renderers/RayCastingModule.h"

namespace CamerasWorkspace {
    inline void renderWorkspaceCamera(Camera &camera, RayCastingModule& module) {
        ImGui::SeparatorText(camera.getName().c_str());

        ImGui::DragFloat("fov", &camera.fov);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);
        ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.01);
        ImGui::DragFloat("Far Plane", &camera.farPlane);

        ImGui::Text("Position:");
        ImGui::DragFloat("x##Position", glm::value_ptr(camera.position) + 0, 0.01f);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);
        ImGui::DragFloat("y##Position", glm::value_ptr(camera.position) + 1, 0.01f);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);
        ImGui::DragFloat("z##Position", glm::value_ptr(camera.position) + 2, 0.01f);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);

        ImGui::Text("Anchor:");
        ImGui::DragFloat("x##Anchor", glm::value_ptr(camera.anchor) + 0, 0.01f);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);
        ImGui::DragFloat("y##Anchor", glm::value_ptr(camera.anchor) + 1, 0.01f);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);
        ImGui::DragFloat("z##Anchor", glm::value_ptr(camera.anchor) + 2, 0.01f);
        if(ImGui::IsItemActive()) module.triggerUpdate(camera);
    }

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

        auto &selected = appState.selectionGroup.getSelectedCamera();
        if(selected) renderWorkspaceCamera(selected->get(), *appState.rayCastingModule);
    }
};

#endif //OPENGL_SANDBOX_CAMERASWORKSPACE_H
