//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_LIGHTSWORKSPACE_H
#define OPENGL_SANDBOX_LIGHTSWORKSPACE_H

#include <glm/gtc/type_ptr.hpp>
#include "../../../../logic/state/AppState.h"
#include "imgui.h"

namespace LightsWorkspace {
    inline void renderWorkspaceLight(Light &light) {
        ImGui::SeparatorText(light.getName().c_str());

        ImGui::DragFloat("strength", &light.getStrengthRef(), 0.01f);

        ImGui::Text("Position:");
        ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(light.getPositionRef())) + 0, 0.01f);
        ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(light.getPositionRef())) + 1, 0.01f);
        ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(light.getPositionRef())) + 2, 0.01f);

        ImGui::ColorPicker3("Color", static_cast<float *>(glm::value_ptr(light.getColorRef())));

        ImGui::Text("Attenuation:");
        ImGui::DragFloat("1##Attenuation", &light.getConstantAttenuationRef(), 0.01f, 0, 1);
        ImGui::DragFloat("d##Attenuation", &light.getLinearAttenuationRef(), 0.01f, 0, 1);
        ImGui::DragFloat("dd##Attenuation", &light.getQuadraticAttenuationRef(), 0.01f, 0, 1);
    }

    inline void render(AppState &appState) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        if(ImGui::BeginChild("Lights#Workspace", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (auto &light : appState.lights) {
                bool isSelected = appState.selectionGroup.getSelectedLight() && &appState.selectionGroup.getSelectedLight()->get() == &light;
                if (ImGui::Selectable(light.getName().c_str(), isSelected)) {
                    appState.selectionGroup.setFocus(light);
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();

        auto &selected = appState.selectionGroup.getSelectedLight();
        if(selected) renderWorkspaceLight(selected->get());
    }
};

#endif //OPENGL_SANDBOX_LIGHTSWORKSPACE_H
