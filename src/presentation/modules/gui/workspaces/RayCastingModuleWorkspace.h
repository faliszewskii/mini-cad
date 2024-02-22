//
// Created by faliszewskii on 22.02.24.
//

#ifndef OPENGL_SANDBOX_RAYCASTINGMODULEWORKSPACE_H
#define OPENGL_SANDBOX_RAYCASTINGMODULEWORKSPACE_H

#include "../../../../logic/state/AppState.h"
#include "../../renderers/RayCastingModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace RayCastingModuleWorkspace {
    inline void render(AppState &appState) {
        auto &module = *appState.rayCastingModule;
        ModulesWorkspaceHelpers::renderModuleToggle(module.active);

        ImGui::SeparatorText("Semi axes");
        ImGui::DragFloat("a", &module.getSemiAxisA(), 0.01);
        if(ImGui::IsItemActive()) module.triggerUpdate();
        ImGui::DragFloat("b", &module.getSemiAxisB(), 0.01);
        if(ImGui::IsItemActive()) module.triggerUpdate();
        ImGui::DragFloat("c", &module.getSemiAxisC(), 0.01);
        if(ImGui::IsItemActive()) module.triggerUpdate();

        ImGui::SeparatorText("Material");
        ImGui::DragFloat("Shininess", &module.getSpecularFactor(), 0.1);
        if(ImGui::IsItemActive()) module.triggerUpdate();

        ImGui::SeparatorText("Ray casting");
        ImGui::DragInt("Starting pixel Size", &module.getStartingPixelSize());
        if(ImGui::IsItemActive()) module.triggerUpdate();
        ImGui::DragInt("Pixels per loop", &module.getPixelsPerLoop());
        if(ImGui::IsItemActive()) module.triggerUpdate();
    }
};

#endif //OPENGL_SANDBOX_RAYCASTINGMODULEWORKSPACE_H
