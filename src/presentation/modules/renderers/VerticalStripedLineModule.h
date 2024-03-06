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

        auto projected = glm::vec3(appState.cursorPosition.x, 0, appState.cursorPosition.z);
        renderStripedLine(projected, appState.cursorPosition, glm::vec4(1, 1, 1, 0.5));

        if(appState.selectedEntities.size() > 1)
            for(auto &el : appState.selectedEntities)
                std::visit(overloaded{
                        [&](Torus &torus){ renderStripedLine(appState.centerOfMassTransformation.translation, torus.transform.translation, glm::vec4(1.f, 0.8f, 0, 0.5f)); },
                        [&](Point &point){ renderStripedLine(appState.centerOfMassTransformation.translation, point.position, glm::vec4(1.f, 0.8f, 0, 0.5f)); }
                }, el.second);
    }

    void renderStripedLine(glm::vec3 begin, glm::vec3 end, glm::vec4 color) const {
        shader.setUniform("beginPointPos", begin);
        shader.setUniform("endPointPos", end);
        shader.setUniform("color", color);
        mesh.render();
    }
};



#endif //OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
