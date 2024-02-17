//
// Created by faliszewskii on 14.02.24.
//

#ifndef OPENGL_SANDBOX_GIZMOMODULE_H
#define OPENGL_SANDBOX_GIZMOMODULE_H

#include <glm/gtc/type_ptr.hpp>
#include "../Module.h"
#include "imgui.h"
#include "../../../../lib/imguizmo/ImGuizmo.h"
#include "../../../logic/algebra/Rect.h"
#include <glm/gtx/matrix_decompose.hpp>

class GizmoModule : public Module {
    const int workspaceWidth;
public:
    explicit GizmoModule(int workspaceWidth) : Module(true), workspaceWidth(workspaceWidth) {}

    void run(AppState &appState) override {
        if (!appState.currentCamera || !appState.selectionGroup.getSelectedTransformTree()) return;
        auto *transformation = &appState.selectionGroup.getSelectedTransformTree().value().get().transform;
        if (transformation == nullptr) return;

        glm::mat4 mat = transformation->getTransformation();
        auto *matrix = static_cast<float *>(glm::value_ptr(mat));

        ImGuiIO &io = ImGui::GetIO();
        ImGuizmo::SetRect(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        ImGuizmo::Manipulate(
                static_cast<const float *>(glm::value_ptr(appState.currentCamera.value().get().getViewMatrix())),
                static_cast<const float *>(glm::value_ptr(appState.currentCamera.value().get().getProjectionMatrix())),
                appState.gizmoOperation, ImGuizmo::LOCAL, matrix, nullptr, nullptr
        );
        transformation->setTransformation(mat);
    }

    [[nodiscard]] std::string getName() const final {
        return "Gizmo Module";
    }
};
#endif //OPENGL_SANDBOX_GIZMOMODULE_H
