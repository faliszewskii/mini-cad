//
// Created by faliszewskii on 14.02.24.
//

#ifndef OPENGL_SANDBOX_GIZMOMODULE_H
#define OPENGL_SANDBOX_GIZMOMODULE_H

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "../../../../lib/imguizmo/ImGuizmo.h"
#include "../../../logic/algebra/Rect.h"
#include <glm/gtx/matrix_decompose.hpp>

class GizmoModule {
    const int workspaceWidth;
public:
    explicit GizmoModule(int workspaceWidth) : workspaceWidth(workspaceWidth) {}

    void run(AppState &appState) {
        if(!appState.guiFocus)
            ImGuizmo::Enable(false);
        else
            ImGuizmo::Enable(true);

        if(appState.selectionGroup.getSelectedPoint())
            gizmoPosition(appState, appState.selectionGroup.getSelectedPoint()->get().position);
    }

    void gizmoPosition(AppState &appState, glm::vec3 &position) {

        glm::mat4 mat = glm::mat4(1.0f);
        mat = glm::translate(mat, position);

        ImGuiIO &io = ImGui::GetIO();
        ImGuizmo::SetRect(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        ImGuizmo::Manipulate(
                static_cast<const float *>(glm::value_ptr(appState.camera.getViewMatrix())),
                static_cast<const float *>(glm::value_ptr(appState.camera.getProjectionMatrix())),
                ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(mat), nullptr, nullptr
        );

        position = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
    }

//    void gizmoTransformTree(AppState &appState) {
//        auto &transformationNode = appState.selectionGroup.getSelectedTransformTree()->get();
//
//        std::vector<glm::mat4> transformationChain; // matrix transformations from first parent to the root.
//        auto parent = transformationNode.getParent();
//        while(parent) {
//            transformationChain.push_back(parent->get().transform.getTransformation());
//            parent = parent->get().getParent();
//        }
//
//        glm::mat4 mat = transformationNode.transform.getTransformation();
//        for(auto &t : transformationChain)
//            mat = t * mat;
//
//        ImGuiIO &io = ImGui::GetIO();
//        ImGuizmo::SetRect(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
//        ImGuizmo::Manipulate(
//                static_cast<const float *>(glm::value_ptr(appState.currentCamera.value().get().getViewMatrix())),
//                static_cast<const float *>(glm::value_ptr(appState.currentCamera.value().get().getProjectionMatrix())),
//                appState.gizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(mat), nullptr, nullptr
//        );
//
//        for(auto &t : std::views::reverse(transformationChain))
//            mat = glm::inverse(t) * mat;
//        transformationNode.transform.setTransformation(mat);
//    }

};
#endif //OPENGL_SANDBOX_GIZMOMODULE_H
