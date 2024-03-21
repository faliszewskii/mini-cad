//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_CENTEROFMASSMODULE_H
#define OPENGL_SANDBOX_CENTEROFMASSMODULE_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/vertices/EmptyVertex.h"
#include "../../../logic/io/IOUtils.h"
#include "RenderHelpers.h"

class CenterOfMassModule {
    const int workspaceWidth;
    Shader shader;
    Mesh<Vertex> mesh;
public:
    CenterOfMassModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
        mesh(Mesh<Vertex>({Vertex(glm::vec3(), glm::vec3(), glm::vec2())}, {}, GL_POINTS)),
        shader(Shader(
                        IOUtils::getResource("shaders/basic/point.vert"),
                        IOUtils::getResource("shaders/basic/depthColor.frag"))) {}

    void run(AppState &appState) {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        if(appState.selectedEntities.size() <= 1) return;
        shader.use();
        shader.setUniform("position", appState.centerOfMassTransformation.translation);
        glPointSize(3);
        shader.setUniform("selected", true);
        RenderHelpers::setUpCamera(appState.camera, shader);
        mesh.render();
        glPointSize(1);
    }
};

#endif //OPENGL_SANDBOX_CENTEROFMASSMODULE_H
