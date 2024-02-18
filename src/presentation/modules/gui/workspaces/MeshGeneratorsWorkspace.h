//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_MESHGENERATORSWORKSPACE_H
#define OPENGL_SANDBOX_MESHGENERATORSWORKSPACE_H

#include "../../../../logic/state/AppState.h"

namespace MeshGeneratorsWorkspace {
    inline void renderWorkspaceGenerator(MeshGenerator &generator) {
        ImGui::SeparatorText(generator.getName().c_str());
        ImGui::Text("Generating for: %s", generator.getTargetMesh().getName().c_str());

        for(auto &parameter : generator.getParameters()) {
            std::visit(overloaded{
                [&parameter](int& value) { ImGui::DragInt(parameter.first.c_str(), &value); },
                [&parameter](float& value) { ImGui::DragFloat(parameter.first.c_str(), &value, 0.01); }
            }, parameter.second);
        }
        generator.generate(); // TODO Check if changed !!!!!
    }

    inline void render(AppState &appState) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        if(ImGui::BeginChild("Generators#Workspace", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (auto &generator : appState.meshGenerators) {
                bool isSelected = appState.selectionGroup.getSelectedMeshGenerator() && &appState.selectionGroup.getSelectedMeshGenerator()->get() == &*generator;
                if (ImGui::Selectable(generator->getName().c_str(), isSelected)) {
                    appState.selectionGroup.setFocus(*generator);
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();

        auto &selected = appState.selectionGroup.getSelectedMeshGenerator();
        if(selected) renderWorkspaceGenerator(selected->get());
    }
};

#endif //OPENGL_SANDBOX_MESHGENERATORSWORKSPACE_H
