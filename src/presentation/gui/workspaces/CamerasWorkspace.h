//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_CAMERASWORKSPACE_H
#define OPENGL_SANDBOX_CAMERASWORKSPACE_H

#include <glm/gtc/type_ptr.hpp>
#include "../../../logic/state/AppState.h"
#include "imgui.h"

namespace CamerasWorkspace {

    inline void render(AppState &appState) {
        auto &camera = appState.camera;
        ImGui::SeparatorText("Camera");

        ImGui::DragFloat("fov", &camera.fov);
        ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.01);
        ImGui::DragFloat("Far Plane", &camera.farPlane);

        ImGui::Text("Position:");
        ImGui::DragFloat("x##Position", glm::value_ptr(camera.position) + 0, 0.01f);
        ImGui::DragFloat("y##Position", glm::value_ptr(camera.position) + 1, 0.01f);
        ImGui::DragFloat("z##Position", glm::value_ptr(camera.position) + 2, 0.01f);

        ImGui::Text("Anchor:");
        ImGui::DragFloat("x##Anchor", glm::value_ptr(camera.anchor) + 0, 0.01f);
        ImGui::DragFloat("y##Anchor", glm::value_ptr(camera.anchor) + 1, 0.01f);
        ImGui::DragFloat("z##Anchor", glm::value_ptr(camera.anchor) + 2, 0.01f);
    }
};

#endif //OPENGL_SANDBOX_CAMERASWORKSPACE_H
