//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_CURSORWORKSPACE_H
#define OPENGL_SANDBOX_CURSORWORKSPACE_H

#include <glm/gtc/type_ptr.hpp>
#include "../../../logic/state/AppState.h"

namespace CursorWorkspace {
    inline void render(AppState &appState) {
        auto position = static_cast<float *>(glm::value_ptr(appState.cursorPosition));

        ImGui::Text("Position:");
        ImGui::DragFloat("x##position", position + 0, 0.01f);
        ImGui::DragFloat("y##position", position + 1, 0.01f);
        ImGui::DragFloat("z##position", position + 2, 0.01f);
    }
};

#endif //OPENGL_SANDBOX_CURSORWORKSPACE_H
