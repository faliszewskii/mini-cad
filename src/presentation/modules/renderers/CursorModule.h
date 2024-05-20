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

        Camera &camera = appState.camera;
        if(camera.stereoscopicVision) {
            glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            auto viewLeft = camera.getViewMatrixStereo(true);
            auto projectionLeft = camera.getProjectionMatrixStereo(true);
            auto viewPosLeft = camera.getViewPositionStereo(true);
            render(camera, viewLeft, viewPosLeft, projectionLeft, appState.cursorPosition);

            glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
            glDepthFunc(GL_LEQUAL);
            auto viewRight = camera.getViewMatrixStereo(false);
            auto projectionRight = camera.getProjectionMatrixStereo(false);
            auto viewPosRight = camera.getViewPositionStereo(false);
            render(camera, viewRight, viewPosRight, projectionRight, appState.cursorPosition);

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        } else {
            auto view = camera.getViewMatrix();
            auto viewPos = camera.getViewPosition();
            auto projection = camera.getProjectionMatrix();
            render(camera, view, viewPos, projection, appState.cursorPosition);
        }

    }

    void render(Camera &camera, glm::mat4 &view, glm::vec3 viewPos, glm::mat4 &projection, glm::vec3 cursorPosition) const {
        shader.use();
        shader.setUniform("position", cursorPosition);
        shader.setUniform("projection", projection);
        shader.setUniform("view", view);
        shader.setUniform("viewPos", viewPos);
        mesh.render();
    }
};

#endif //OPENGL_SANDBOX_CURSORMODULE_H
