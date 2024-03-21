//
// Created by faliszewskii on 04.03.24.
//

#ifndef OPENGL_SANDBOX_CURSORMODULE_H
#define OPENGL_SANDBOX_CURSORMODULE_H

#include "../../../logic/entities/shader/Shader.h"
#include "../../../logic/vertices/EmptyVertex.h"
#include "../../../logic/entities/mesh/Mesh.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "RenderHelpers.h"

class CursorModule {
    const int workspaceWidth;
    Shader shader;
    Mesh<EmptyVertex> mesh;

public:
    CursorModule(int workspaceWidth) : workspaceWidth(workspaceWidth), mesh(Mesh<EmptyVertex>({EmptyVertex{}}, {}, GL_POINTS)),
        shader(Shader(
                      IOUtils::getResource("shaders/basic/empty.vert"),
                      IOUtils::getResource("shaders/cursor/cursor.geom"),
                      IOUtils::getResource("shaders/cursor/cursor.frag"))) {}

    void run(AppState &appState) {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        shader.use();
        shader.setUniform("position", appState.cursorPosition);
        RenderHelpers::setUpCamera(appState.camera, shader);
        mesh.render();
    }
};

#endif //OPENGL_SANDBOX_CURSORMODULE_H
