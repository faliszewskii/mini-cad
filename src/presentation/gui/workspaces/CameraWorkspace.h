//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_CAMERAWORKSPACE_H
#define OPENGL_SANDBOX_CAMERAWORKSPACE_H

#include <glm/gtc/type_ptr.hpp>
#include "../../../logic/state/AppState.h"
#include "imgui.h"

namespace CameraWorkspace {

    inline void render(AppState &appState) {
        auto &camera = appState.camera;
        ImGui::SeparatorText("Camera");

        ImGui::DragFloat("fov", &camera.fov);
        ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.01);
        ImGui::DragFloat("Far Plane", &camera.farPlane);

        ImGui::Text("Position:");
        ImGui::DragScalar("x##Position", ImGuiDataType_Double, glm::value_ptr(camera.position) + 0, 0.01f);
        ImGui::DragScalar("y##Position", ImGuiDataType_Double, glm::value_ptr(camera.position) + 1, 0.01f);
        ImGui::DragScalar("z##Position", ImGuiDataType_Double, glm::value_ptr(camera.position) + 2, 0.01f);

        ImGui::Text("Anchor:");
        bool anchorModified = false;
        glm::vec<3, double> anchor = camera.anchor;
        anchorModified |= ImGui::DragScalar("x##Anchor", ImGuiDataType_Double, glm::value_ptr(anchor) + 0, 0.01f);
        anchorModified |= ImGui::DragScalar("y##Anchor", ImGuiDataType_Double, glm::value_ptr(anchor) + 1, 0.01f);
        anchorModified |= ImGui::DragScalar("z##Anchor", ImGuiDataType_Double, glm::value_ptr(anchor) + 2, 0.01f);
        if(anchorModified) {
            camera.position += anchor - camera.anchor;
            camera.anchor = anchor;
        }
    }
};

#endif //OPENGL_SANDBOX_CAMERAWORKSPACE_H
