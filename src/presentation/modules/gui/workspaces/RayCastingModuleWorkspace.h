//
// Created by faliszewskii on 22.02.24.
//

#ifndef OPENGL_SANDBOX_RAYCASTINGMODULEWORKSPACE_H
#define OPENGL_SANDBOX_RAYCASTINGMODULEWORKSPACE_H

#include "../../../../logic/state/AppState.h"
#include "../../renderers/RayCastingModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace RayCastingModuleWorkspace {

    inline void displayMatrix(glm::mat4 M, const std::string& name, bool &isOpen) {
        ImGui::Checkbox(name.c_str(), &isOpen);
        if(isOpen) {
            ImGui::BeginDisabled();
            ImGui::PushItemWidth(ImGui::GetFontSize()*4);
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++) {
                    ImGui::InputFloat("##dipslay", glm::value_ptr(M) + j*4 + i);
                    if(j!=3) ImGui::SameLine();
                }
            ImGui::PopItemWidth();
            ImGui::EndDisabled();
        }
    }

    inline void render(AppState &appState) {
        auto &module = *appState.rayCastingModule;
        ModulesWorkspaceHelpers::renderModuleToggle(module.active);

        ImGui::SeparatorText("Semi axes");
        ImGui::DragFloat("a", &module.getSemiAxisA(), 0.01);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("b", &module.getSemiAxisB(), 0.01);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("c", &module.getSemiAxisC(), 0.01);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());

        ImGui::Text("Translation:");
        ImGui::DragFloat("x##translation", glm::value_ptr(module.getTranslation()) + 0, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("y##translation", glm::value_ptr(module.getTranslation()) + 1, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("z##translation", glm::value_ptr(module.getTranslation()) + 2, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::Text("Euler rotation:");
        ImGui::DragFloat("x##rotation", glm::value_ptr(module.getEulerRotation()) + 0, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("y##rotation", glm::value_ptr(module.getEulerRotation()) + 1, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("z##rotation", glm::value_ptr(module.getEulerRotation()) + 2, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::Text("Scale:");
        ImGui::DragFloat("x##scale", glm::value_ptr(module.getScale()) + 0, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("y##scale", glm::value_ptr(module.getScale()) + 1, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());
        ImGui::DragFloat("z##scale", glm::value_ptr(module.getScale()) + 2, 0.01f);
        if(ImGui::IsItemActive() && appState.currentCamera) module.triggerUpdate(appState.currentCamera.value());

        ImGui::SeparatorText("Material");
        ImGui::DragFloat("Shininess", &module.getSpecularFactor(), 0.1);

        ImGui::SeparatorText("Ray casting");
        ImGui::DragInt("Starting pixel Size", &module.getStartingPixelSize());
        ImGui::DragInt("Pixels per loop", &module.getPixelsPerLoop());
        ImGui::Checkbox("First pass full", &module.getFirstPassFull());

        ImGui::SeparatorText("Debug");
        static bool isM, isInvM, isD, isDHalf, isDPrim, isA, isB, isC;
        displayMatrix(module.M, "M", isM);
        displayMatrix(module.invM, "invM", isInvM);
        displayMatrix(module.D, "D", isD);
        displayMatrix(module.DPrim, "DPrim", isDPrim);
        displayMatrix(module.A, "A", isA);
        displayMatrix(module.B, "B", isB);
        displayMatrix(module.C, "C", isC);
    }
};

#endif //OPENGL_SANDBOX_RAYCASTINGMODULEWORKSPACE_H
