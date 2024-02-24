//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_MATERIALSWORKSPACE_H
#define OPENGL_SANDBOX_MATERIALSWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "imgui.h"

namespace MaterialsWorkspace {
    inline void renderWorkspaceMaterial(Material &material) {
        ImGui::SeparatorText(material.getName().c_str());

        if (!material.getHint().empty())
            ImGui::Text("AssImp hint: %s", material.getHint().c_str());

        ImGui::DragFloat("Shininess", &material.getShininessRef(), 0.01f, 0); // TODO To Phong property.

        ImGui::ColorPicker4("Albedo", static_cast<float *>(glm::value_ptr(material.getAlbedoRef())));

        // TODO Material UI
    }

    inline void render(AppState &appState) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        if(ImGui::BeginChild("Materials#Workspace", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (auto &material : appState.materials) {
                bool isSelected = appState.selectionGroup.getSelectedMaterial() && &appState.selectionGroup.getSelectedMaterial()->get() == &*material;
                if (ImGui::Selectable(material->getName().c_str(), isSelected)) {
                    appState.selectionGroup.setFocus(*material);
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();

        auto &selected = appState.selectionGroup.getSelectedMaterial();
        if(selected) renderWorkspaceMaterial(selected->get());
    }
};

#endif //OPENGL_SANDBOX_MATERIALSWORKSPACE_H
