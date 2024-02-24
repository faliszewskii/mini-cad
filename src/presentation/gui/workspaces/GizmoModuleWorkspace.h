//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_GIZMOMODULEWORKSPACE_H
#define OPENGL_SANDBOX_GIZMOMODULEWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "../../modules/gui/GizmoModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace GizmoModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.gizmoModule->active);

        static std::map<ImGuizmo::OPERATION, std::string> operations {
                {ImGuizmo::OPERATION::UNIVERSAL, "Universal"},
                {ImGuizmo::OPERATION::TRANSLATE, "Translate"},
                {ImGuizmo::OPERATION::ROTATE, "Rotate"},
                {ImGuizmo::OPERATION::SCALE, "Scale"}
        };

        if (ImGui::BeginCombo("Operations", operations[appState.gizmoOperation].c_str()))
        {
            for(const auto &operation : operations) {
                if (ImGui::Selectable(operation.second.c_str(), appState.gizmoOperation == operation.first))
                    appState.gizmoOperation = operation.first;
            }
            ImGui::EndCombo();
        }
    }
};

#endif //OPENGL_SANDBOX_GIZMOMODULEWORKSPACE_H
