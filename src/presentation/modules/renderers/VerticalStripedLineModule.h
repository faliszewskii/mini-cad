//
// Created by faliszewskii on 29.02.24.
//

#ifndef OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
#define OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H

#include "../../../logic/entities/shader/Shader.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/vertices/EmptyVertex.h"
#include "../../../logic/entities/mesh/Mesh.h"
#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "RenderHelpers.h"

class VerticalStripedLineModule {
    const int workspaceWidth;
    Shader shader;
    Mesh<EmptyVertex> mesh;

public:
    explicit VerticalStripedLineModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
        mesh(Mesh<EmptyVertex>({EmptyVertex{}}, {}, GL_POINTS)), // TODO I don't need names here. Maybe they are not neede anywhere at all.
        shader(Shader("verticalLine",
                        IOUtils::getResource("shaders/basic/empty.vert"),
                        IOUtils::getResource("shaders/verticalLine/verticalLine.geom"),
                        IOUtils::getResource("shaders/verticalLine/verticalLine.frag"))) {}

    void run(AppState &appState) {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        shader.use();
        RenderHelpers::setUpCamera(appState.camera, shader);
        shader.setUniform("time", float(glfwGetTime()));

        renderVerticalLine(appState.cursorPosition);
    }

    void renderVerticalLine(glm::vec3 position) const {
        shader.setUniform("endPointPos", position);
        mesh.render();
    }
};



#endif //OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
